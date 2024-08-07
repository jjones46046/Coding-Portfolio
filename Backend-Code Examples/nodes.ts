// Purpose: This file contains the cron job for the nodes table. It retrieves nodes from XIQ, processes them, and writes the data into D1.

import type { AppEnv } from '~/index';
import type { ExtremeIDReturn } from '~/types';

import { xiqFetch, xiqLogin } from '~/utils';

export type NodeEntry = {
  // xiq_nodeId: number;
  locationId: string;
  serial: string;
  mac: string;
  model: string;
  lastSeen?: Date | null;
  kind: 'AP' | 'EDGE' | 'SWITCH' | 'VM';
  name: string;
  status: boolean;
  tenantId?: string | null;
};

export async function cronNodes(env: AppEnv) {
  const access_token = await xiqLogin(env);
  if (!access_token) {
    throw new Error('nodeDataIngest - Failed to get access token');
  }

  //Grabbing the first 100 devices from XIQ then using the page data to grab the rest.
  //Also grabbing the total page count, so we know how many pages to loop through later for the rest of the data.
  const response = (await xiqFetch(
    'devices?limit=100&connected=true&views=basic,location&page=1',
    'GET',
    access_token,
    env
  )) as ExtremeIDReturn;
  if (response.error_message) {
    throw new Error('nodeDataIngest - Failed to get page count');
  }

  // Place the data on the queue for further processing
  await env.BUTLER_QUEUE.send({
    property: 'nodes',
    action: 'process',
    data: response.data
  });

  //Grabbing the rest of the devices by looping through each page.
  //Starting at 2 because we already grabbed the first page.
  for (let i = 2; i <= response.total_pages; i++) {
    const res = (await xiqFetch(
      'devices?limit=100&connected=true&views=basic,location&page=' + i,
      'GET',
      access_token,
      env
    )) as ExtremeIDReturn;
    if (res.error_message) {
      throw new Error(`nodeDataIngest. Error: ${JSON.stringify(res.error_message, null, 2)}`);
    }
    // Place the data on the queue for further processing
    await env.BUTLER_QUEUE.send({
      property: 'nodes',
      action: 'process',
      data: res.data
    });
  }
}
