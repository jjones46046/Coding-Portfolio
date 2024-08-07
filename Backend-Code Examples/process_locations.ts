// Purpose: This file is used to process locations from the locations api and insert them into the database.

import type { AppEnv } from '~/index';

import { locations, tenants } from '@fulcrum/shared/schema/fulcrum';
import { getDB } from '~/db';
import { eq } from 'drizzle-orm';

import 'drizzle-zod';

import { z } from 'zod';

const LocationTypeLabel = z.enum(['Store', 'Warehouse', 'Distribution Center', 'Head Office', 'Office']);

export type locAddresses = {
  address_city: string;
  address_line1: string;
  address_email: string;
  address_line2: string;
  address_name: string;
  address_state: string;
  address_type_id: string;
  address_zip_code: string;
  country_id?: number;
};
export type locData = {
  addresses: locAddresses[];
  active_flag: boolean;
  channel: string;
  location_code: string;
  district?: string;
  location_type_label: string;
  chain: string;
  region: string;
  location_name: string;
};

export const processD1Locations = async (e: AppEnv): Promise<void> => {
  const db = getDB(e);
  const result = await db.select({ id: tenants.id }).from(tenants).where(eq(tenants.sanitizedName, 'dtlr'));
  if (result.length <= 0 || !result || typeof result === 'undefined') {
    throw new Error('No tenant found');
  }

  const response = await fetch(e.LOCATIONSAPI_URL, {
    headers: {
      'Content-Type': 'application/json',
      'CF-Access-Client-Id': e.LOCATIONSAPI_CLIENT_ID,
      'CF-Access-Client-Secret': e.LOCATIONSAPI_CLIENT_SECRET,
      Connection: 'keep-alive'
    }
  });

  if (!response.ok) {
    throw new Error('Failed to fetch locations from locations api');
  }

  const locationData = await response.json<locData[]>();
  const locations_filtered = locationData.filter(
    (item) =>
      item.active_flag &&
      item.region !== 'Distribution Center' &&
      item.region !== 'E-Commerce' &&
      item.location_name.toUpperCase() !== 'PROMO USE ONLY' &&
      item.location_name.toUpperCase() !== 'SA REQUIRED' &&
      item.location_name.toUpperCase() !== 'DO NOT USE'
  );

  for (const loc of locations_filtered) {
    // these pull all current locations from the database
    const tmp_loc : typeof locations.$inferInsert = {
      ...loc,
      name: loc.location_name,
      code: loc.location_code,
      countryId: loc.addresses[0]?.country_id,
      locationTypeLabel: loc.location_type_label,
      status: loc.active_flag ? 'active' : 'inactive',
      addressCity: loc.addresses[0]!.address_city, // Fallback to empty string if undefined
      addressLine1: loc.addresses[0]!.address_line1,
      addressEmail: loc.addresses[0]!.address_email,
      addressLine2: loc.addresses[0]!.address_line2,
      addressName: loc.addresses[0]!.address_name,
      addressState: loc.addresses[0]!.address_state,
      addressZipCode: loc.addresses[0]!.address_zip_code,
      tenantId: result[0]?.id
    }

    await db
      .insert(locations)
      .values(tmp_loc)
      .onConflictDoUpdate({
        target: locations.code,
        set: {
          name: tmp_loc.name,
          channel: tmp_loc.channel,
          district: tmp_loc.district,
          region: tmp_loc.region,
          locationTypeLabel: tmp_loc.locationTypeLabel,
          status: tmp_loc.status,
          addressCity: tmp_loc.addressCity,
          addressLine1: tmp_loc.addressLine1,
          addressEmail: tmp_loc.addressEmail,
          addressLine2: tmp_loc.addressLine2,
          addressName: tmp_loc.addressName,
          addressState: tmp_loc.addressState,
          addressZipCode: tmp_loc.addressZipCode
        }
      });
  }
};
