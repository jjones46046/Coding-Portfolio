import type { AppEnv } from '@context/OdinProvider';
import type { SelectLocationFull } from '@fulcrum/shared/schema/fulcrum';
import type {
  DNS_Delete_Response,
  DNS_Query_Response,
  mwan_lan,
  mwan_lan_return
} from '@fulcrum/shared/types/mwan_sites';

import { locations, mwan_site_reservations, mwan_sites } from '@fulcrum/shared/schema/fulcrum';
import { eq } from 'drizzle-orm';

import { getDB } from '@services/db.server';

export async function deleteReservation(env: AppEnv, locUUID: string, reserveID: string) {
  /* -------------------
  Quick Input Validation
  ------------------- */

  if (!env || !locUUID || !reserveID) {
    console.log('One of the input parameters is not valid');
    console.log('locUUID: ' + locUUID);
    console.log('reserveID: ' + reserveID);
    throw new Response('Input is missing, check logs', { status: 400 });
  }

  const db = getDB(env);
  const loc: SelectLocationFull | undefined = await db.query.locations.findFirst({
    where: eq(locations.id, locUUID),
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

  if (!loc) {
    console.log('Variable `loc` failed a truthy test');
    throw new Response('Location not found', { status: 400 });
  }

  /* ----------------------------
  Delete CF Magic LAN Reservation
  ---------------------------- */

  const reserveRow = loc.mwanSites.mwanSiteReservations.find((element) => element.id == reserveID);

  if (!reserveRow) {
    console.log('The reservation could not be found in the mwan_site_reservations DB');
    throw new Response('Reservation not found', { status: 400 });
  }
  if (!reserveRow.mwanLanId) {
    console.log('The LAN ID was falsy, check the database to make sure it is accurate');
    throw new Response('LAN ID not found', { status: 400 });
  }
  if (!reserveRow.mwanSiteId) {
    console.log('The LAN ID was falsy, check the database to make sure it is accurate');
    throw new Response('LAN ID not found', { status: 400 });
  }
  const lan_response: mwan_lan_return = await QueryLanAPI(env, reserveRow.mwanSiteId, reserveRow?.mwanLanId, 'GET');

  if (!lan_response.result) {
    console.log('No results were returned from the LAN query' + JSON.stringify(lan_response.errors));
    throw new Response('No LAN interface found', { status: 400 });
  }
  const lan_info = lan_response.result;

  if (!reserveRow.reservationsMac) {
    console.log('No MAC address was found for the reservation in the DB');
    throw new Response('No MAC address found');
  }
  const db_mac_formatted = FormatMAC(reserveRow.reservationsMac?.toLowerCase());

  const body = {
    name: lan_info.name,
    nat: lan_info.nat,
    physport: lan_info.physport,
    static_addressing: lan_info.static_addressing,
    vlan_tag: lan_info.vlan_tag
  };
  if (!body) {
    console.log('There is a problem with the interface configuration');
    throw new Response('The interface is not properly configured, advise NetOps', { status: 400 });
  }
  if (!body.static_addressing) {
    console.log('There is a problem with the interface configuration');
    throw new Response('The interface is not properly configured, advise NetOps', { status: 400 });
  }
  if (!body.static_addressing?.dhcp_server) {
    console.log('There is a problem with the interface configuration');
    throw new Response('The interface is not properly configured, advise NetOps', { status: 400 });
  }
  if (!body.static_addressing?.dhcp_server.reservations) {
    console.log('There is a problem with the interface configuration');
    throw new Response('The interface is not properly configured, advise NetOps', { status: 400 });
  }
  if (!body.static_addressing?.dhcp_server.reservations[db_mac_formatted]) {
    console.log('There is a problem with the interface configuration');
    throw new Response('The interface is not properly configured, advise NetOps', { status: 400 });
  }

  delete body.static_addressing?.dhcp_server.reservations[db_mac_formatted];

  const delete_response: mwan_lan_return = await QueryLanAPI(
    env,
    reserveRow.mwanSiteId,
    reserveRow.mwanLanId,
    'PUT',
    body
  );

  if (!delete_response.success) {
    console.log('Delete failed, error provided: ' + delete_response.errors);
    throw new Response('Deleting the DHCP reservation failed, contact NetOps');
  }

  /* ----------------
  DNS Record Deletion
  ---------------- */

  if (!reserveRow.dnsRecord) {
    console.log('The reseveration does not have a DNS record ID');
    throw new Response('The DNS record ID is not present in the database', { status: 400 });
  }

  const delete_dns: DNS_Delete_Response = await DNS_Record_Delete(env, reserveRow.dnsRecord);

  if (reserveRow.reservationsType == 'printer') {
    const cname = `dlr${loc.code}.prt${reserveRow.name.slice(8, 9)}.store.dtlronline.com`;
    const cnameDeletion = await DNS_Record_Query(env, cname);
    if (cnameDeletion) console.log('Successfully deleted CNAME');
  }

  if (!delete_dns.success) {
    console.log('DNS deletion failed and returned the following errors: ' + JSON.stringify(delete_dns.errors));
    throw new Response('The DNS record could not be deleted, notify NetOps');
  }

  /* -------------
  Database Removal
  ------------- */
  console.log('Starting DB Record delete');

  const deleteDbRecord = await db
    .delete(mwan_site_reservations)
    .where(eq(mwan_site_reservations.id, reserveRow.id))
    // .returning({ deletedId: mwan_site_reservations.id });
    .returning();
  if (deleteDbRecord[0]?.id) {
    console.log('Successfully deleted Record');
  }

  /* -------------------
  Update mwan_lans field
  ------------------  */
  const update_lans = await UpdateMwanLans(env, reserveRow.mwanSiteId, reserveRow.mwanLanId, locUUID);
  if (update_lans.status == 200) console.log('Successful');
}
async function UpdateMwanLans(
  env: AppEnv,
  mwan_site_id: string,
  mwan_lan_id: string,
  locationUUID: string
): Promise<Response> {
  const cf_api_base = 'https://api.cloudflare.com/client/v4';
  const cf_lans_api =
    cf_api_base + '/accounts/' + env.CLOUDFLARE_ACCOUNT_ID + '/magic/sites/' + mwan_site_id + '/lans/' + mwan_lan_id;
  const headers = {
    'X-Auth-Email': env.CLOUDFLARE_EMAIL,
    'X-Auth-Key': env.CLOUDFLARE_GLOBAL_API_KEY
  };
  const response = await fetch(cf_lans_api, {
    method: 'GET',
    headers: headers
  });
  const { result, errors, success }: mwan_lan_return = await response.json();
  if (!success) {
    console.log('GET Request to the LANs API failed: ' + JSON.stringify(errors));
    throw new Response('Querying from the LANs API failed, contact NetOps');
  }

  if (!result) {
    throw new Response('LAN interface query failed to complete, contact NetOps');
  }

  const db = getDB(env);

  const site = await db
    .select({ siteID: mwan_sites.mwanSiteId, mwan_lans: mwan_sites.mwanLans })
    .from(mwan_sites)
    .where(eq(mwan_sites.locationId, locationUUID));
  if (!site[0]) {
    console.log('Error site not found');
    throw new Response('Error site not found');
  }
  if (!site[0]?.mwan_lans) {
    console.log('Error');
    throw new Response('Error site not found');
  }
  const lans = site[0].mwan_lans;
  const updatedLans: mwan_lan = lans.filter((lan) => lan.vlan_tag !== 100);
  updatedLans.push(result);
  const update_mwan_lans = await db
    .update(mwan_sites)
    .set({ mwanLans: updatedLans })
    .where(eq(mwan_sites.mwanSiteId, mwan_site_id))
    .returning();

  if (!update_mwan_lans[0]?.mwanLans) {
    console.log('Database update failed validate function');
    throw new Response('Database update failed, contact NetOps');
  }
  return new Response('Update successful', { status: 200 });
}

async function QueryLanAPI(
  env: AppEnv,
  mwan_site_id: string,
  mwan_lan_id: string,
  method: string,
  body?: {}
): Promise<any> {
  const cf_api_base = 'https://api.cloudflare.com/client/v4';
  const cf_lans_api =
    cf_api_base + '/accounts/' + env.CLOUDFLARE_ACCOUNT_ID + '/magic/sites/' + mwan_site_id + '/lans/' + mwan_lan_id;
  const headers = {
    'X-Auth-Email': env.CLOUDFLARE_EMAIL,
    'X-Auth-Key': env.CLOUDFLARE_GLOBAL_API_KEY
  };

  if (method == 'GET') {
    const response = await fetch(cf_lans_api, {
      method: method,
      headers: headers
    });
    const json_response: mwan_lan_return = await response.json();
    return json_response;
  } else {
    const response = await fetch(cf_lans_api, {
      method: method,
      headers: headers,
      body: JSON.stringify(body)
    });
    const json_response: mwan_lan_return = await response.json();
    return json_response;
  }
}

function FormatMAC(mac: string): string {
  if (mac.length !== 12 || !mac) throw new Error('Invalid MAC Address format');
  return mac.match(/.{1,2}/g)?.join(':') ?? '';
}

async function DNS_Record_Delete(env: AppEnv, dns_record_id: string): Promise<any> {
  const headers = {
    'X-Auth-Email': env.CLOUDFLARE_EMAIL,
    'X-Auth-Key': env.CLOUDFLARE_GLOBAL_API_KEY
  };
  const dns_base_api = 'https://api.cloudflare.com/client/v4';
  const dns_api_full = dns_base_api + '/zones/' + env.CLOUDFLARE_DTLRONLINE_ZONE + '/dns_records/' + dns_record_id;
  const dns_delete_request = await fetch(dns_api_full, {
    method: 'DELETE',
    headers: headers
  });
  const dns_response: DNS_Delete_Response = await dns_delete_request.json();
  return dns_response;
}

async function DNS_Record_Query(env: AppEnv, cname: string): Promise<boolean> {
  const headers = {
    'X-Auth-Email': env.CLOUDFLARE_EMAIL,
    'X-Auth-Key': env.CLOUDFLARE_GLOBAL_API_KEY
  };
  const dns_base_api = 'https://api.cloudflare.com/client/v4';
  const dns_api_full = `${dns_base_api}/zones/${env.CLOUDFLARE_DTLRONLINE_ZONE}/dns_records?name=${cname}`;
  const dns_request = await fetch(dns_api_full, {
    method: 'GET',
    headers: headers
  });
  const { result, errors, success }: DNS_Query_Response = await dns_request.json();
  if (!success) {
    console.log('CNAME Deletion returned an error: ' + errors);
    throw new Response('CNAME Deletion failed contact NetOps');
  }
  const deleteCName = await DNS_Record_Delete(env, result[0].id);
  if (!deleteCName) {
    console.log('Record deletion failed');
    throw new Response('CNAME record deletion failed, contact NetOps');
  }
  return true;
}