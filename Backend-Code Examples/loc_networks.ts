import type { AppEnv } from '~/index';

import { locations } from '@fulcrum/shared/schema/fulcrum';
import { getDB } from '~/db';
import { eq } from 'drizzle-orm';

const storeNetBase = '172.17.0.0/12';

export async function massLocationNetworks(e: AppEnv) {
  //Callable by the queue to process all locations in the database.
  const db = getDB(e);
  let index: number = 0;
  const locationData = await db
    .select({ locationCode: locations.code, exIpSchema: locations.ipSchema, exAptosSubnet: locations.aptosNetwork })
    .from(locations)
    .where(eq(locations.locationTypeLabel, 'Store'))
    .orderBy(locations.code);
  let existingStoreNet: string[] = [];

  //Assemble an array of the existing subnets using D1
  locationData.forEach((loc) => {
    if (loc.exIpSchema) {
      existingStoreNet.push(...loc.exIpSchema);
    }
  });

  for (const loc of locationData) {
    // Initialize newEntries object with all possible keys
    const newEntries: { ipSchema?: string[]; aptosNetwork?: string; updatedAt?: Date } = {};

    // If the location does not have an existing IP schema, find a new one
    if (!loc.exIpSchema) {
      const newIpSchema = findFirstNonMatchingNetwork(storeNetBase, existingStoreNet, 24);
      if (!newIpSchema) {
        throw new Error(
          'Error: No available /24 subnets for location code: ' +
            loc.locationCode +
            ' List of existing subnets: ' +
            existingStoreNet
        );
      }
      if (newIpSchema.startsWith('172.32')) {
        throw new Error('Error: Reached end of private address range at: ' + newIpSchema);
      }
      // Insert the new subnet into the existingStoreNet array at the correct index to avoid conflicts
      //Replace the old array with the new one
      existingStoreNet = [...existingStoreNet.slice(0, index), newIpSchema, ...existingStoreNet.slice(index)];
      newEntries.ipSchema = [newIpSchema];
    }
    //Calculate the Aptos network based on the location code and update the updatedAt field
    newEntries.aptosNetwork = locationCodeToNetwork(loc.locationCode.trim());
    newEntries.updatedAt = new Date();

    await db.update(locations).set(newEntries).where(eq(locations.code, loc.locationCode));
    index++;
  }
}

function findFirstNonMatchingNetwork(baseCidr: string, existingCidrs: string[], subnetSize: number): string | null {
  const [baseIp, basePrefix] = baseCidr.split('/');
  if (!baseIp || !basePrefix) {
    throw new Error('Invalid CIDR');
  }
  const basePrefixNum = parseInt(basePrefix, 10); //basePrefixNum is obtained by converting the prefix length of the base CIDR to an integer.
  const numHostsInBase = Math.pow(2, 32 - basePrefixNum); // Total hosts in base network
  const numHostsInSubnet = Math.pow(2, 32 - subnetSize); // Hosts in the specified subnet
  const ipNum = ipToNumber(baseIp);
  let existingIndex = 0;

  for (let i = 0; i < numHostsInBase; i += numHostsInSubnet) {
    const subnetIp = numberToIp(ipNum + i);
    const subnetCidr = `${subnetIp}/${subnetSize}`;

    if (existingIndex < existingCidrs.length && existingCidrs[existingIndex] == subnetCidr) {
      existingIndex++;
    } else {
      return subnetCidr;
    }
  }
  // If all networks match, return null
  return null;
}

function locationCodeToNetwork(locationCode: string) {
  const baseIp = parseInt(locationCode) >= 3000 ? '10.66.0.192/26' : '10.67.0.192/26';
  let thirdOctet = locationCode.slice(-3);

  if (parseInt(thirdOctet) > 255) {
    thirdOctet = '1' + thirdOctet.slice(0, 2); // Prepend '1' in front of the two middle digits
  }

  // Remove leading zeros if the third octet has them.
  thirdOctet = thirdOctet.replace(/^0+/, '');

  // Constructing the IP address
  return baseIp.replace(/(\d+\.\d+\.)\d+(\.\d+)/, `$1${thirdOctet}$2`);
}

function ipToNumber(ip: string): number {
  return ip.split('.').reduce((acc, octet) => (acc << 8) + parseInt(octet, 10), 0);
}
function numberToIp(num: number): string {
  return [num >>> 24, (num >> 16) & 255, (num >> 8) & 255, num & 255].join('.');
}
