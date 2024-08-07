import type { AppEnv } from '@context/OdinProvider';
import type { CloudflareResponse } from './connector';

import { connectors, mwan_site_reservations, mwan_sites } from '@fulcrum/shared/schema/fulcrum';
import { getDB } from '@services/db.server';
import { eq } from 'drizzle-orm';

export async function cleanseBrokenSite(env: AppEnv, locationUUID: string) {
  const db = getDB(env);
  console.log(locationUUID)
  const site = await db.select({ mwanSiteID: mwan_sites.mwanSiteId, connectorId: mwan_sites.connectorId }).from(mwan_sites).where(eq(mwan_sites.locationId, locationUUID));
  if (!site[0] || !site[0].connectorId || !site[0].mwanSiteID) {
    throw new Error('Location site ID or connector ID not found');
  }
  const connectorID = site[0].connectorId.trim();
  const siteID = site[0].mwanSiteID.trim();

  const reservations = await db.select().from(mwan_site_reservations).where(eq(mwan_site_reservations.mwanSiteId, siteID));

  // Delete each DNS record associated with the site.
  if (reservations.length > 0) {
    reservations.forEach(async element => {
      const url = `https://api.cloudflare.com/client/v4/zones/${env.CLOUDFLARE_DTLRONLINE_ZONE}/dns_records/${element.dnsRecord}`;

      const response = await fetch(url, {
        method: 'DELETE',
        headers: {
          'Content-Type': 'application/json',
          'X-Auth-Email': env.CLOUDFLARE_EMAIL,
          'X-Auth-Key': env.CLOUDFLARE_GLOBAL_API_KEY
        },
      });

      const data: CloudflareResponse = await response.json();
      console.log(JSON.stringify(data, null, 2));
      if (data.success == false) {
        throw new Error('Failed to delete DNS record: ' + JSON.stringify(data.errors));
      }
    });
  }
  // Delete all reservations associated with the site.
  await db.delete(mwan_site_reservations)
    .where(eq(mwan_site_reservations.mwanSiteId, siteID.trim()))
    .catch((error) => {
      throw new Error('Issue updating D1 mwanSiteReservations table.\n' + JSON.stringify(error, null, 2));
    });


  // Deactivate the connector and reset it to being a default DTLR connector.
  const optionsConnector = {
    method: 'PUT',
    headers: {
      'Content-Type': 'application/json',
      'X-Auth-Email': env.CLOUDFLARE_EMAIL,
      'X-Auth-Key': env.CLOUDFLARE_GLOBAL_API_KEY
    },
    body: JSON.stringify({
      activated: false,
      notes: 'DTLR'
    })
  };

  try {
    const response = await fetch(
      'https://api.cloudflare.com/client/v4/accounts/' + env.CLOUDFLARE_ACCOUNT_ID + '/magic/connectors/' + connectorID, optionsConnector
    );
    const connectorUpdate: CloudflareResponse = await response.json();

    if (!connectorUpdate.success) {
      throw new Error('Failed to de-activate connector with error: \n' + JSON.stringify(connectorUpdate.errors));
    }
  } catch (err) {
    throw new Error('' + err);
  }

   //Remove the old locationId from the connectors table, set locationCode, activeDevice, and description back to default.
   await db.update(connectors)
   .set({
     locationId: null,
     locationCode: 'Available',
     activeDevice: 'Inactive',
     description: 'DTLR',
     deletedAt: new Date()
   })
   .where(eq(connectors.locationId, locationUUID))
   .catch((error) => {
     throw new Error('Issue updating D1 connectors table.\n' + JSON.stringify(error, null, 2));
   });

  //Delete the site now that the connector is deactivated.
  let cleanseResponse: CloudflareResponse;
  const options = {
    method: 'DELETE',
    headers: {
      'Content-Type': 'application/json',
      'X-Auth-Email': env.CLOUDFLARE_EMAIL,
      'X-Auth-Key': env.CLOUDFLARE_GLOBAL_API_KEY
    }
  };

  try {
    const response = await fetch(
      'https://api.cloudflare.com/client/v4/accounts/' + env.CLOUDFLARE_ACCOUNT_ID + '/magic/sites/' + siteID,
      options
    );

    cleanseResponse = await response.json();
    if (!cleanseResponse.success) {
      throw new Error('Failed to delete site, request failed with error: \n' + JSON.stringify(cleanseResponse.errors));
    }
  } catch (err) {
    throw new Error('' + err);
  }

  //Remove the old mwanSite from D1.
  await db.delete(mwan_sites)
    .where(eq(mwan_sites.locationId, locationUUID))
    .catch((error) => {
      throw new Error('Issue updating D1 mwanSite table.\n' + JSON.stringify(error, null, 2));
    });
}
