import type { mwan_lan } from '@fulcrum/shared/types/mwan_sites';
import type { AppEnv } from '~/index';
import { mwan_sites } from '@fulcrum/shared/schema/fulcrum';

import { getDB } from '~/db';
import { CloudflareResponse } from './connector';
import { eq } from 'drizzle-orm';

type TunnelHealthCheck = {
  avg: {
    tunnelState: number; // Tunnel state is a number
  };
  dimensions: {
    tunnelName: string;
    siteName: string;
  };
};

type TunnelTraffic = {
    dimensions: {
      tunnelName: string;
    },
    sum: {
      bits: number;
    }
};

type ApiResponse = {
  data: {
    viewer: {
      accounts: {
        magicTransitTunnelHealthChecksAdaptiveGroups: TunnelHealthCheck[];
        magicTransitTunnelTrafficAdaptiveGroups: TunnelTraffic[];
      }[];
    };
  };
};

export async function grabLans(env: AppEnv, mwanSiteId: string) {
  const db = getDB(env);
  let lanResponse: CloudflareResponse;

  //Attempt to update the Secure LAN with the new reservation using the Cloudflare API.
  const options = {
    method: 'GET',
    headers: {
      'Content-Type': 'application/json',
      'X-Auth-Email': env.CLOUDFLARE_EMAIL,
      'X-Auth-Key': env.CLOUDFLARE_GLOBAL_API_KEY
    },
  };

  try {
    const response = await fetch('https://api.cloudflare.com/client/v4/accounts/' + env.CLOUDFLARE_ACCOUNT_ID + '/magic/sites/' + mwanSiteId + '/lans', options);
    lanResponse = await response.json();

    if (!lanResponse.success) {
      throw 'Failed to update LAN, request failed with error: \n' + JSON.stringify(lanResponse.errors);
    }
    //Add the updated Secure LAN back to the LANs array.
    console.log(JSON.stringify(lanResponse.result as unknown as mwan_lan, null, 2));
    await db.update(mwan_sites).set({mwanLans: lanResponse.result as unknown as mwan_lan}).where(eq(mwan_sites.mwanSiteId, mwanSiteId));
  }
  catch (error) {
    console.error(error);
  }
}

type siteTunnelStatus = {
  [siteName: string]: {
    "primaryStatus": number;
    "backupStatus": number;
    "primaryBandwith": number;
    "backupBandwith": number;
  };
};

export async function tunnelHealth(env: AppEnv, start_time: string, end_time: string) {
  const query: string =
    `query GetTunnelHealthCheckResults($accountTag: String!, $datetimeStart: String!, $datetimeEnd: String!) {
  viewer {
    accounts(filter: {accountTag: $accountTag}) {
      magicTransitTunnelHealthChecksAdaptiveGroups(
        limit: 100,
        filter: {
          datetime_geq: $datetimeStart,
          datetime_lt: $datetimeEnd
        }
        orderBy: [tunnelName_ASC]
      ) {
        avg {
          tunnelState
        }
        dimensions {
          tunnelName
          siteName
        }
      }
      magicTransitTunnelTrafficAdaptiveGroups(
          limit: 100,
          filter: {
            datetime_geq: $datetimeStart,
            datetime_lt: $datetimeEnd
          }
        ) {
          sum {
            bits
          }
          dimensions {
            tunnelName
          }
        }
    }
  }
}`;

  // Define the variables with updated datetime values
  // ISO 8601 format (YYYY-MM-DDTHH:mm:ss.sssZ)
  const variables = {
    accountTag: env.CLOUDFLARE_ACCOUNT_ID,
    datetimeStart: start_time,
    datetimeEnd: end_time
  };

  // Define the request payload
  const payload = {
    query,
    variables
  };

  // Define the endpoint and headers
  const endpoint = "https://api.cloudflare.com/client/v4/graphql";
  const headers = {
    "Content-Type": "application/json",
    "X-Auth-Email": env.CLOUDFLARE_EMAIL,
    "X-Auth-Key": env.CLOUDFLARE_GLOBAL_API_KEY
  };

  // Function to send the GraphQL request using fetch
  try {
    const response = await fetch(endpoint, {
      method: 'POST',
      headers: headers,
      body: JSON.stringify(payload)
    });

    if (!response.ok) {
      throw new Error(`HTTP error! status: ${response.status}`);
    }

    const data:ApiResponse = await response.json();
    const healthChecks:TunnelHealthCheck[] = data.data.viewer.accounts[0]!.magicTransitTunnelHealthChecksAdaptiveGroups;
    const trafficChecks:TunnelTraffic[] = data.data.viewer.accounts[0]!.magicTransitTunnelTrafficAdaptiveGroups;

    const flattenedData = flattenData(healthChecks, trafficChecks);
    console.log("Response data:", JSON.stringify(flattenedData, null, 2));
  } catch (error) {
    console.error("Error fetching data:", error);
  }

  function flattenData(data: TunnelHealthCheck[], data2: TunnelTraffic[]): siteTunnelStatus {
    return data.reduce((acc, item) => {
      const { siteName, tunnelName } = item.dimensions;
      const tunnelState = item.avg.tunnelState;

      if (!acc[siteName]) {
        // Initialize with default values
        acc[siteName] = { primaryStatus: 1.5, backupStatus: 1.5, primaryBandwith
    : 0, backupBandwith: 0 }; //Setting to a default value of 1.5 to indicate no data
      }

      if (tunnelName.includes("Primary") || tunnelName.includes("Comcast")) {
        acc[siteName]!.primaryStatus = tunnelState;
        acc[siteName]!.primaryBandwith
   = data2.find((item) => item.dimensions.tunnelName === tunnelName)?.sum.bits || 0;
        
      } else if (tunnelName.includes("Backup") || tunnelName.includes("Cradlepoint")) {
        acc[siteName]!.backupStatus = tunnelState;
        acc[siteName]!.backupBandwith = data2.find((item) => item.dimensions.tunnelName === tunnelName)?.sum.bits || 0
      }

      return acc;
    }, {} as siteTunnelStatus);
  }
}