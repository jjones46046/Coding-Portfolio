import type { AppEnv } from '~/index';

import { connectors, locations, tenants } from '@fulcrum/shared/schema/fulcrum';
import { getDB } from '~/db';
import { eq } from 'drizzle-orm';

export type ConnectorResult = {
  id: string;
  activated: boolean;
  notes: string;
  device: {
    serial_number: string;
  };
};

export type CloudflareResponse = {
  errors: [];
  messages: [];
  result: ConnectorResult[];
  success: boolean;
};

export async function grabConnectors(env: AppEnv) {
  const db = getDB(env);
  let resultPackage: CloudflareResponse;
  let connectorData: ConnectorResult[] = [];

  //Attempt to create a site as well as connect the connector using the Cloudflare API.
  const options = {
    method: 'GET',
    headers: {
      'Content-Type': 'application/json',
      'X-Auth-Email': env.CLOUDFLARE_EMAIL,
      'X-Auth-Key': env.CLOUDFLARE_GLOBAL_API_KEY
    }
  };

  try {
    const response = await fetch(
      'https://api.cloudflare.com/client/v4/accounts/' + env.CLOUDFLARE_ACCOUNT_ID + '/magic/connectors',
      options
    );
    resultPackage = await response.json();

    if (!resultPackage.success) {
      throw new Error('Failed to create site, request failed with error: \n' + JSON.stringify(resultPackage.errors));
    }
    connectorData = resultPackage.result;
  } catch (err) {
    throw new Error('' + err);
  }

  for (const connector of connectorData) {
    let activated = 'Inactive';
    let locCode: string = 'Available';
    let locUUID: string = '';

    if (connector.activated) {
      activated = 'Active';
    }

    if (connector.notes !== 'DTLR' && connector.activated) {
      const testCode = connector.notes.trim().split('-')[1]!;

      //Check if the location code is valid
      if (testCode.length == 4) {
        const location = await db.select({ id: locations.id }).from(locations).where(eq(locations.code, testCode));

        if (!location[0]) {
          throw new Error('Location not found');
        }
        await db.update(locations).set({ updatedAt: new Date() }).where(eq(locations.code, testCode));
        //Set the location code and UUID for the connector
        locCode = testCode;
        locUUID = location[0].id;
      }
      else {
        locCode = 'Unknown Location';
      }
    }

    const [tenant] = await db
      .select({ id: tenants.id })
      .from(tenants)
      .where(eq(tenants.sanitizedName, 'DTLR'.toLowerCase()));

    if (!tenant) throw new Error('Tenant not found');

    //Create a new connector in the database.
    await db
      .insert(connectors)
      .values({
        id: connector.id,
        activeDevice: activated,
        description: connector.notes,
        serial: connector.device.serial_number,
        locationCode: locCode,
        locationId: locUUID,
        tenantId: tenant.id
      })
      .onConflictDoUpdate({
        target: connectors.serial,
        set: {
          updatedAt: new Date(),
          activeDevice: activated,
          description: connector.notes,
          locationCode: locCode,
          locationId: locUUID,
          tenantId: tenant.id
        }
      });
  }
}
