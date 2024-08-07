// import type { SelectLocationFull } from '@fulcrum/shared/schema/fulcrum';

import type { lan_response, mwan_lan, mwan_wan } from '@fulcrum/shared/types/mwan_sites';
import type { AppEnv } from '@context/OdinProvider';

import { connectors, locations, mwan_sites } from '@fulcrum/shared/schema/fulcrum';
import { getDB } from '@services/db.server';
import { eq } from 'drizzle-orm';

/**
 * The application should receive the following data from the frontend:
 *  - Connector ID (connector will also be listed on cloudflare)
 *  - Location UUID
 * In order the application will complete the following tasks:
 *  - Create the site using the Cloudflare API.
 *  - Create the primary and backup WANs for the site.
 *  - Divide the subnet into 9 subnets.
 *  - Create the LANs for the site.
 *  - Update the location table with the connector ID and cloudflare site ID.
 *  - Update the connector table with the location ID.
 */

export type CloudflareResponse = {
  errors: [];
  messages: [];
  result: lan_response;
  success: boolean;
};

export async function createMagicWanSite(env: AppEnv, locationUUID: string, serial: string) {
  const db = getDB(env);
  const location = await db.select().from(locations).where(eq(locations.id, locationUUID));
  const connectorInfo = await db.select({ id: connectors.id }).from(connectors).where(eq(connectors.serial, serial));

  if (!location || !location[0] || !connectorInfo[0]) {
    throw new Error('Location or connector not found');
  }

  const connectorID = connectorInfo[0].id.trim();
  let newSiteData: CloudflareResponse;

  //Attempt to create a site as well as connect the connector using the Cloudflare API.
  const options = {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
      'X-Auth-Email': env.CLOUDFLARE_EMAIL,
      'X-Auth-Key': env.CLOUDFLARE_GLOBAL_API_KEY
    },
    body: JSON.stringify({
      name: ('DTLR-' + location[0].code).trim(),
      description: location[0].name,
      connector_id: connectorID
    })
  };

  try {
    const response = await fetch(
      'https://api.cloudflare.com/client/v4/accounts/' + env.CLOUDFLARE_ACCOUNT_ID + '/magic/sites',
      options
    );
    newSiteData = await response.json();

    if (!newSiteData.success) {
      throw new Error('Failed to create site, request failed with error: \n' + JSON.stringify(newSiteData.errors));
    }
  } catch (err) {
    throw new Error('' + err);
  }

  //After the site is created, update the connector to be activated.
  const optionsConnector = {
    method: 'PUT',
    headers: {
      'Content-Type': 'application/json',
      'X-Auth-Email': env.CLOUDFLARE_EMAIL,
      'X-Auth-Key': env.CLOUDFLARE_GLOBAL_API_KEY
    },
    body: JSON.stringify({
      activated: true,
      notes: 'DTLR-' + location[0].code
    })
  };

  try {
    const response = await fetch(
      'https://api.cloudflare.com/client/v4/accounts/' + env.CLOUDFLARE_ACCOUNT_ID + '/magic/connectors/' + connectorID,
      optionsConnector
    );
    const connectorUpdate: CloudflareResponse = await response.json();

    if (!connectorUpdate.success) {
      throw new Error('Failed to activate connector with error: \n' + JSON.stringify(connectorUpdate.errors));
    }
  } catch (err) {
    throw new Error('' + err);
  }

  const newSiteId = newSiteData.result.id;

  //Update the connector table with the location ID as the connector is now associated with a location.
  await db
    .update(connectors)
    .set({
      locationId: locationUUID,
      locationCode: location[0].code,
      description: 'DTLR-' + location[0].code,
      activeDevice: 'Active',
      deletedAt: null
    })
    .where(eq(connectors.id, connectorID))
    .then(() => {})
    .catch((error) => {
      throw new Error('Issue updating D1 connectors table.\n' + JSON.stringify(error, null, 2));
    });

  //Creating the primary and backup WANs for the site.
  const setupWANs = [
    { newSiteId: newSiteId, physport: 5, name: 'Primary', priority: 10 },
    { newSiteId: newSiteId, physport: 6, name: 'Backup', priority: 100 }
  ];

  let wanArray: mwan_wan = [];

  for (const wan of setupWANs) {
    wanArray = await createWAN(wan.newSiteId, wan.physport, wan.name, wan.priority, wanArray, env);
  }

  //Create an initial entry for the new site in the mwan_sites table.
  const siteEntry: typeof mwan_sites.$inferInsert = {
    mwanWans: wanArray,
    mwanSiteId: newSiteId.trim(),
    locationId: locationUUID,
    connectorId: connectorID,
    name: location[0].name,
    locationLat: location[0].latitude as unknown as string,
    locationLong: location[0].longitude as unknown as string
  };

  const site = await db
    .insert(mwan_sites)
    .values(siteEntry)
    .returning({ siteId: mwan_sites.id })
    .catch((error) => {
      throw new Error('Issue updating D1 mwanSites table.\n' + JSON.stringify(error, null, 2));
    });
  if (!site[0]?.siteId) {
    throw new Error('Failed to create new site');
  }

  const subnets: string[] = divideSubnet(location[0].ipSchema![0]!);
  const aptos_network = location[0].aptosNetwork;
  // const VLAN50 = aptos_network?.split('.192/26')[0] + '.0/26'

  if (!subnets || !aptos_network) {
    throw new Error('Failed to divide subnet');
  }

  const setupLANs = [
    { baseSubnet: subnets[0], prefix: subnets[0]!.split('/')[1], description: 'VND_EQP_20', vlan_tag: 20, physport: 1 }, //26
    { baseSubnet: subnets[1], prefix: subnets[1]!.split('/')[1], description: 'GUEST_40', vlan_tag: 40, physport: 1 }, //26
    { baseSubnet: subnets[2], prefix: subnets[2]!.split('/')[1], description: 'VOIP_18', vlan_tag: 18, physport: 1 }, //27
    { baseSubnet: subnets[3], prefix: subnets[3]!.split('/')[1], description: 'SW_MGMT', vlan_tag: 0, physport: 3 }, //27
    {
      baseSubnet: subnets[4],
      prefix: subnets[4]!.split('/')[1],
      description: 'COMPUTE_130',
      vlan_tag: 130,
      physport: 2
    }, //27
    { baseSubnet: subnets[5], prefix: subnets[5]!.split('/')[1], description: 'IPMI_10', vlan_tag: 0, physport: 4 }, //27
    {
      baseSubnet: aptos_network,
      prefix: aptos_network.split('/')[1],
      description: 'SECURE',
      vlan_tag: 100,
      physport: 1
    } //26
    //Change VLAN 50 to the same as the aptos_network (VLAN 100) but change the end to 0 instead of 192. For example, if a location has aptos_network 10.67.179.192/26
  ];

  let lanArray: mwan_lan = [];
  for (const lan of setupLANs) {
    lanArray = await createLAN(
      lan.baseSubnet!,
      lan.prefix!,
      newSiteId,
      lan.description,
      lan.vlan_tag,
      lan.physport,
      lanArray,
      env
    );
  }

  //Update the mwan_sites table with the LANs.
  await db
    .update(mwan_sites)
    .set({
      mwanLans: lanArray
    })
    .where(eq(mwan_sites.id, site[0].siteId))
    .catch((error) => {
      throw new Error('Issue updating D1 mwan_site table.\n' + JSON.stringify(error, null, 2));
    });
}

async function createWAN(
  newSiteId: string,
  physport: number,
  name: string,
  priority: number,
  wan_storage: mwan_wan,
  env: AppEnv
) {
  let wanResponse: CloudflareResponse;
  const options = {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
      'X-Auth-Email': env.CLOUDFLARE_EMAIL,
      'X-Auth-Key': env.CLOUDFLARE_GLOBAL_API_KEY
    },
    body: JSON.stringify({
      site_id: newSiteId,
      name: name,
      physport: physport, //Primary 5 backup 6
      priority: priority, //Primary 10 backup 100
      vlan_tag: 0
    })
  };

  try {
    const response = await fetch(
      'https://api.cloudflare.com/client/v4/accounts/' +
        env.CLOUDFLARE_ACCOUNT_ID +
        '/magic/sites/' +
        newSiteId +
        '/wans',
      options
    );
    wanResponse = await response.json();
    if (!wanResponse.success) {
      throw new Error('Failed to create WAN, request failed with error: \n' + JSON.stringify(wanResponse.errors));
    }
    const wanData = {
      site_id: newSiteId,
      name: name,
      physport: physport, //Primary 5 backup 6
      priority: 100,
      vlan_tag: 0
    };

    wan_storage.push(wanData);
    return wan_storage;
  } catch (err) {
    throw new Error('' + err);
  }
}

async function createLAN(
  baseSubnet: string,
  prefix: string,
  newSiteId: string,
  description: string,
  vlan_tag: number,
  physport: number,
  lan_storage: mwan_lan,
  env: AppEnv
) {
  const prefixMap: { [key: string]: number } = {
    '25': 126,
    '26': 62,
    '27': 30
  };
  const numericalSubnet = ipToNumber(baseSubnet.split('/')[0]!);
  const staticAddress = numberToIp(numericalSubnet + 1);
  const firstUseable = vlan_tag === 100 ? numberToIp(numericalSubnet + 18) : numberToIp(numericalSubnet + 2); // Adding one because the first address is the network address.
  const lastUseable = numberToIp(numericalSubnet + prefixMap[prefix]!);
  let lanResponse: CloudflareResponse;

  const options = {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
      'X-Auth-Email': env.CLOUDFLARE_EMAIL,
      'X-Auth-Key': env.CLOUDFLARE_GLOBAL_API_KEY
    },
    body: JSON.stringify({
      physport: physport,
      vlan_tag: vlan_tag,
      name: description,
      static_addressing: {
        address: staticAddress + '/' + prefix,
        dhcp_server: {
          dns_server: '1.1.1.1',
          dhcp_pool_start: firstUseable,
          dhcp_pool_end: lastUseable
        }
      },
      nat: {
        static_prefix: baseSubnet
      }
    })
  };

  try {
    const response = await fetch(
      'https://api.cloudflare.com/client/v4/accounts/' +
        env.CLOUDFLARE_ACCOUNT_ID +
        '/magic/sites/' +
        newSiteId +
        '/lans',
      options
    );
    lanResponse = await response.json();

    if (!lanResponse.success) {
      throw new Error('Failed to create LAN, request failed with error: \n' + JSON.stringify(lanResponse.errors));
    }

    const lanData: lan_response = lanResponse.result;
    lan_storage.push(lanData);
    return lan_storage;
  } catch (err) {
    throw new Error('' + err);
  }
}

function divideSubnet(subnet: string): string[] {
  // Divide the /24 subnet into 3 /26s and 4 /27s
  const [ip, prefix] = subnet.split('/');

  if (!ip || prefix?.trim() != '24') {
    throw new Error('Invalid Subnet');
  }

  // Define the sizes and prefix lengths for the subnets
  const sizes = [64, 64, 32, 32, 32, 32]; // /26, /26, /27, /27, /27, /27
  const prefixs = [26, 26, 27, 27, 27, 27];

  const subnets: string[] = [];
  let currentIpNum = ipToNumber(ip);
  //Iterate through each subnet size and calculate the next IP address
  for (const size of sizes) {
    const subnetIp = numberToIp(currentIpNum);
    subnets.push(`${subnetIp}/${prefixs[sizes.indexOf(size)]}`);
    currentIpNum += size;
  }

  return subnets;
}

function ipToNumber(ip: string): number {
  return ip.split('.').reduce((acc, octet) => (acc << 8) + parseInt(octet, 10), 0);
}
function numberToIp(num: number): string {
  return [num >>> 24, (num >> 16) & 255, (num >> 8) & 255, num & 255].join('.');
}


