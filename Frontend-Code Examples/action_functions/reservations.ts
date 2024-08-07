import type { lan_response, mwan_lan } from '@fulcrum/shared/types/mwan_sites';
import type { AppEnv } from '@context/OdinProvider';

import { locations, mwan_site_reservations, mwan_sites } from '@fulcrum/shared/schema/fulcrum';
import { getDB } from '@services/db.server';
import { eq } from 'drizzle-orm';

export type CloudflareResponse = {
  errors: [];
  messages: [];
  result: lan_response;
  success: boolean;
};

// Define the type for the DNS record
export type DnsRecord = {
  content: string;
  name: string;
  proxied: boolean;
  type: string;
  comment: string;
  tags: string[];
  ttl: number;
};

// Define the type for the API response
export type RecordResponse = {
  success: boolean;
  errors: [];
  messages: [];
  result: {
    id: string;
    [key: string]: unknown;
  };
};

interface Reservations {
  [mac_address: string]: string;
}

export async function createReservation(env: AppEnv, locationUUID: string, macAddress: string, deviceType: string) {
  const db = getDB(env);
  const reservation = await db
    .select()
    .from(mwan_site_reservations)
    .where(eq(mwan_site_reservations.reservationsMac, macAddress));
  if (reservation.length > 0) {
    throw new Error('A reservation already exists for this MAC address');
  }
  const location = await db.select().from(locations).where(eq(locations.id, locationUUID));
  const site = await db
    .select({ siteID: mwan_sites.mwanSiteId, mwan_lans: mwan_sites.mwanLans })
    .from(mwan_sites)
    .where(eq(mwan_sites.locationId, locationUUID));

  if (!location[0] || !site[0]) {
    throw new Error('Location, node, or site not found');
  }

  //Don't remove this trim, it's necessary for the Cloudflare API.
  const siteID = site[0].siteID?.trim();
  const lans = site[0].mwan_lans;
  const locationCode = location[0].code.trim();

  if (!siteID || !lans) {
    throw new Error('Site or LANs not found');
  }

  // Find the LAN with vlan_tag 100
  const secureLan: lan_response | undefined = lans.find((lan) => lan.vlan_tag === 100) as unknown as lan_response;

  if (!secureLan) {
    throw new Error('LAN with vlan_tag 100 not found');
  }

  // Filter out the LAN with vlan_tag 100
  const updatedLans: mwan_lan = lans.filter((lan) => lan.vlan_tag !== 100);

  if (!secureLan.static_addressing.dhcp_server.reservations) {
    // Initialize the reservations object if it doesn't exist
    secureLan.static_addressing.dhcp_server.reservations = {};
  }

  const startingNumber = ipToNumber(secureLan.static_addressing.address);
  const reservationsStart = numberToIp(startingNumber + 1);
  // Add the new reservation to the reservations object
  const returnedArray = getNextReservationIP(
    reservationsStart,
    deviceType as 'printer' | 'zebra' | 'pinpad',
    secureLan.static_addressing.dhcp_server.reservations
  );
  const reservationIP = returnedArray[0];
  const deviceName = returnedArray[1];
  if (!reservationIP || !deviceName) {
    throw new Error('No available slots for device type');
  }
  secureLan.static_addressing.dhcp_server.reservations[formaatMacAddress(macAddress)] = reservationIP;

  let lanResponse: CloudflareResponse;
  // Remove the id category from the Secure LAN object to avoid conflicts with the Cloudflare API.
  const { id, ...secureLanWithoutId } = secureLan;

  //Attempt to update the Secure LAN with the new reservation using the Cloudflare API.
  const options = {
    method: 'PUT',
    headers: {
      'Content-Type': 'application/json',
      'X-Auth-Email': env.CLOUDFLARE_EMAIL,
      'X-Auth-Key': env.CLOUDFLARE_GLOBAL_API_KEY
    },
    body: JSON.stringify(secureLanWithoutId)
  };

  try {
    const response = await fetch(
      'https://api.cloudflare.com/client/v4/accounts/' +
        env.CLOUDFLARE_ACCOUNT_ID +
        '/magic/sites/' +
        siteID +
        '/lans/' +
        id,
      options
    );
    lanResponse = await response.json();

    if (!lanResponse.success) {
      throw 'Failed to update LAN, request failed with error: \n' + JSON.stringify(lanResponse.errors);
    }
    //Add the updated Secure LAN back to the LANs array.
    updatedLans.push(lanResponse.result);

    const dnsRecord:DnsRecord = {
      content: reservationIP,
      name: deviceName + '.' + 's' + locationCode,
      proxied: false,
      type: 'A',
      comment: 'Record for ' + deviceName + ' at ' + locationCode,
      tags: ['dtlronline', 'connector', 'device', locationCode],
      ttl: 3600
    };
    //create DNS record
    const dnsUUID = await createDnsRecord(
      env.CLOUDFLARE_DTLRONLINE_ZONE,
      env.CLOUDFLARE_EMAIL,
      env.CLOUDFLARE_GLOBAL_API_KEY,
      dnsRecord
    );

    //Update the D1 mwan_site table with the updated LANs including the new reservation.

    //mixing async and await pattenrs ////
    await db
      .update(mwan_sites)
      .set({
        mwanLans: updatedLans
      })
      .where(eq(mwan_sites.mwanSiteId, siteID));

    await db.insert(mwan_site_reservations).values({
      mwanSiteId: siteID,
      reservationsMac: macAddress,
      reservationsIp: reservationIP,
      reservationsType: deviceType,
      mwanLanId: id,
      name: deviceName + '.' + 's' + locationCode,
      dnsRecord: dnsUUID
    });
  } catch (err) {
    throw new Error('' + err);
  }
}

function getNextReservationIP(
  reservationPoolStart: string,
  deviceType: 'zebra' | 'printer' | 'pinpad',
  reservations: Reservations
): string[] {
  // Convert IP address to a number
  const startAddress = ipToNumber(reservationPoolStart);

  // Max slots for each device type
  const maxSlots = {
    printer: 2,
    zebra: 5,
    pinpad: 5
  };

  // Order and offsets for device types
  const typeOffsets = {
    printer: 0,
    zebra: maxSlots.printer,
    pinpad: maxSlots.printer + maxSlots.zebra
  };

  // Initialize reservation slots array
  const reservationSlots = new Array(12).fill(null);

  //Fill the reservation slots based on current reservations.
  //The slot index is calculated by subtracting the start address from the IP address.
  Object.values(reservations).forEach((ip_address) => {
    const ipAddress = ipToNumber(ip_address);
    const slotIndex = ipAddress - startAddress;
    if (slotIndex >= 0 && slotIndex < reservationSlots.length) {
      reservationSlots[slotIndex] = ip_address;
    }
  });

  //Find next available slot for the device type by going through the reservation slots array in the slots determined by the device type.
  const offset = typeOffsets[deviceType];

  for (let i = 0; i < maxSlots[deviceType]; i++) {
    const slotIndex = offset + i;
    if (!reservationSlots[slotIndex]) {
      const reservationAddress = startAddress + slotIndex;
      const deviceName = deviceType + '-' + (i + 1);
      return [numberToIp(reservationAddress), deviceName];
    }
  }
  throw new Error('No available slots for device type: ' + deviceType);
}

function ipToNumber(ip: string): number {
  return ip.split('.').reduce((acc, octet) => (acc << 8) + parseInt(octet, 10), 0);
}
function numberToIp(num: number): string {
  return [num >>> 24, (num >> 16) & 255, (num >> 8) & 255, num & 255].join('.');
}
function formaatMacAddress(macAddress: string): string {
  return (
    macAddress
      .toLowerCase()
      .match(/.{1,2}/g)
      ?.join(':') || ''
  );
}

// Define the function to create a DNS record
async function createDnsRecord(
  zoneId: string,
  authEmail: string,
  authKey: string,
  dnsRecord: DnsRecord
): Promise<string> {
  const url = `https://api.cloudflare.com/client/v4/zones/${zoneId}/dns_records`;

  const response = await fetch(url, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
      'X-Auth-Email': authEmail,
      'X-Auth-Key': authKey
    },
    body: JSON.stringify(dnsRecord)
  });

  const data: RecordResponse = await response.json();

  if (!data.success) {
    throw new Error('Failed to create DNS record: ' + JSON.stringify(data.errors));
  }

  return data.result.id;
}
