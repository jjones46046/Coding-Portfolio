import type { SelectLocationFull } from '@fulcrum/shared/schema/fulcrum';
import type { AppEnv } from '~/index';
import type { DeviceFeed } from '~/types';

import { locations, mwan_site_reservations } from '@fulcrum/shared/schema/fulcrum';
import { getDB } from '~/db';
import { eq } from 'drizzle-orm';

export type DnsRecord = {
  content: string | string[];
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

export type Reservation = {
  id: string;
  mwanSiteId: string;
  reservationsMac: string;
  reservationsIp: string;
  reservationsType: string;
  dnsRecord: string;
  mwanLanId: string;
  name: string;
};

export async function createReservationNew(env: AppEnv, data: DeviceFeed) {
  try {
    const db = getDB(env);

    const siteData: SelectLocationFull | undefined = await db.query.locations.findFirst({
      where: eq(locations.id, data.locationCode),
      with: {
        connectors: true,
        nodes: true,
        tenant: true,
        mwanSites: {
          with: {
            mwanSiteReservations: true
          }
        }
      }
    });
    if (!siteData) {
      throw new Error('Location, node, or site not found');
    }
    const progNameGen = siteData.mwanSites.mwanSiteReservations.filter(
      (res) => res.reservationsType === data.deviceType
    ).length;

    // Assuming siteData is defined and has the correct structure

    const reservationsIps: string | string[] = siteData.mwanSites.mwanSiteReservations.map((reservation) =>
      String(reservation.reservationsIp)
    );

    const aptosNetwork = siteData.aptosNetwork;
    if (!aptosNetwork) {
      throw new Error('No network found for location or network is invalid');
    }
    // Extract the first three octets from aptosNetwork

    const firstThreeOctets = aptosNetwork.split('/')[0].split('.').slice(0, 3) ?? [];
    const baseIp = firstThreeOctets.join('.');

    // Find the first usable address
    const firstUsableAddress = (baseIp: string, start: number, end: number): string => {
      for (let i = start; i <= end; i++) {
        const candidateIp = `${baseIp}.${i}`;
        if (reservationsIps.indexOf(candidateIp) === -1) {
          return candidateIp;
        }
      }
      return '';
    };

    // Ternary operation
    const resid = reservationsIps.length < 0 ? reservationsIps : firstUsableAddress(baseIp, 194, 210);

    const dnsRecord: DnsRecord = {
      content: resid,
      name: data.deviceType + '.' + 's' + siteData.code,
      proxied: false,
      type: 'A',
      comment: 'Record for ' + data.deviceType + ' at ' + siteData.code,
      tags: ['dtlronline', data.deviceType, 'device', siteData.code, 'fulcrum'],
      ttl: 300
    };
    //create DNS record

    const url = `https://api.cloudflare.com/client/v4/zones/${env.CLOUDFLARE_DTLRONLINE_ZONE}/dns_records`;

    const cfResp = await fetch(url, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        'X-Auth-Email': env.CLOUDFLARE_EMAIL,
        'X-Auth-Key': env.CLOUDFLARE_GLOBAL_API_KEY
      },
      body: JSON.stringify(dnsRecord)
    });

    if (cfResp.status !== 200) {
      return;
    }
    const resObj: RecordResponse = await cfResp.json();

    await db.insert(mwan_site_reservations).values({
      mwanSiteId: siteData.mwanSites.id,
      reservationsMac: data.mac,
      reservationsIp: resid,
      reservationsType: data.deviceType,
      mwanLanId: siteData.mwanSites.id,
      name: data.deviceType + '.' + 's' + siteData.code,
      dnsRecord: resObj.result.id
    });
    // .catch((error) => {
    //   throw new Error('Issue inserting into D1 mwan_site_reservations table.\n' + JSON.stringify(error, null, 2));
    // });
  } catch (err) {
    throw new Error('' + err);
  }
}
