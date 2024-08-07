// Purpose: Synchronize locations with data in XIQ. Will create a new location in XIQ if it does not exist, and update the location ID in the database at the end of both steps.

import type { AppEnv } from '~/index';

import { locations } from '@fulcrum/shared/schema/fulcrum';
import { getDB } from '~/db';
import { xiqFetch, xiqLogin } from '~/utils';
import { eq } from 'drizzle-orm';

// Response structure expected from XIQ API calls
export type XiqResponse = {
  id?: number;
  error_code?: string;
  error_id?: string;
  error_message?: string;
};

//Function to synchronize locations with XIQ
export async function cronXiqLocations(env: AppEnv) {
  // Initialize the Drizzle database
  const db = getDB(env);

  // Retrieve the access token from XIQ
  const access_token = await xiqLogin(env);
  if (!access_token) {
    throw new Error('nodeDataIngest - Failed to get access token');
  }

  const nodes_loc = await db.select().from(locations);

  // Iterate through each location in the database
  for (const site of nodes_loc) {
    if (site.xiqLocId) {
      // Location already exists in XIQ, retrieve its details
      continue;
    }
    // Create a location object to be sent to XIQ
    const locationObject = {
      name: site.code,
      parent_id: env.EXTREME_ORG_ID,
      country_code: 840
    };

    // Call XIQ API to create a new location or retrieve existing location details
    const response = (await xiqFetch('locations/site', 'POST', access_token, env, locationObject)) as XiqResponse;

    // Check if the location already exists in XIQ
    const xiq_location_id: number | undefined = response.id;

    if (response.error_code) {
      // Log error if location creation fails and continue to the next iteration
      throw new Error('Error while creating location:' + site.code + response.error_message);
    }

    // Location created successfully, add code for adding a building to the new site
    const buildingObject = {
      name: site.code + '-building',
      parent_id: xiq_location_id,
      address: {
        address: site.addressLine1,
        city: site.addressCity,
        state: site.addressState,
        postal_code: site.addressZipCode
      }
    };

    // Call XIQ API to create a new building
    const buildingResponse = (await xiqFetch(
      'locations/building',
      'POST',
      access_token,
      env,
      buildingObject
    )) as XiqResponse;

    if (buildingResponse.error_code) {
      // Log error if building creation fails and continue to the next iteration
      console.log('Error while creating building on location#:' + site.code + buildingResponse.error_message);
      continue;
    }

    // Create a floor object to be sent to XIQ
    const floorObject = {
      parent_id: buildingResponse.id,
      name: 'Floor 1',
      db_attenuation: 1.0,
      measurement_unit: 'FEET',
      installation_height: 2.2,
      map_size_width: 10.0,
      map_size_height: 10.0
    };

    // Call XIQ API to create a new floor
    const floorResponse = (await xiqFetch('locations/floor', 'POST', access_token, env, floorObject)) as XiqResponse;

    if (floorResponse.error_code) {
      // Log error if floor creation fails and continue to the next iteration
      console.log('Error while creating floor on location:' + JSON.stringify(floorResponse, null, 2));
      continue;
    }

    // Update the location ID in the database at the end of both steps
    await db
      .update(locations)
      .set({ xiqLocId: xiq_location_id, updatedAt: new Date() })
      .where(eq(locations.code, site.code));
  }
}
