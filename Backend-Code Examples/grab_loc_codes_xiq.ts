//Purpose: This function sends a get location tree request to XIQ, then iterates through the response filling in XIQ ids in the nodes table.

import type { AppEnv } from '~/index';

import { locations } from '@fulcrum/shared/schema/fulcrum';
import { getDB } from '~/db';
import { xiqFetch, xiqLogin } from '~/utils';
import { eq } from 'drizzle-orm';

// Response structure expected from XIQ API calls
export type XiqResponse = {
  id: number;
  org_id: number;
  name: string;
  unique_name: string;
  address: string;
  children: XiqResponse[];
};

export async function grabLocCodesXiq(env: AppEnv) {
  // Send a get location tree request to XIQ, then iterate through the first index of the response to grab the XIQ ID.
  const db = getDB(env);

  const access_token = await xiqLogin(env);
  if (!access_token) {
    throw new Error('nodeDataIngest - Failed to get access token');
  }

  const response = (await xiqFetch('locations/tree', 'GET', access_token, env)) as XiqResponse[];

  //Fix the possible undefined error
  if (response[0] === undefined) {
    throw new Error('nodeDataIngest - Failed to get location tree');
  }

  const locationsArray: XiqResponse[] = response[0].children;
  const floorsByLocationName: { [locationName: string]: { xiqFloorId: number; xiqLocId: number } } = {};

  locationsArray.forEach((loc) => {
    const floor = loc.children[0]?.children[0]?.id; //The floor is the level we need for device assignments. It is located at the .children.children level of each location tree.
    const location = loc.id; //The location ID is the parent of the floor ID.
    if (floor === undefined) {
      return;
    }
    floorsByLocationName[loc.name] = { xiqFloorId: floor, xiqLocId: location }; //Add the floor to the object with the location ID as the key
  });

  for (const locationName in floorsByLocationName) {
    //Update the locations table with the XIQ Floor ID.
    await db
      .update(locations)
      .set({
        xiqFloorId: floorsByLocationName[locationName]?.xiqFloorId,
        xiqLocId: floorsByLocationName[locationName]?.xiqLocId,
        updatedAt: new Date()
      })
      .where(eq(locations.code, locationName));
  }
}
