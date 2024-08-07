import type { SelectLocationFull } from '@fulcrum/shared/schema/fulcrum';
import type { XIQClient } from '@services/xiq.server';

import { CardDetail } from '@components/CardDetail';
import { SectionHeader } from '@components/SectionHeader';

export function TabInformation({ loc }: { loc: SelectLocationFull; xiq_clients: XIQClient }) {
  const numberOfNodes = loc.nodes ? loc.nodes.length : 0;
  return (
    <>
      <div role="tabpanel" className="tab-content w-full py-10">
        <div
          id={'location-extra-info'}
          className={'border-neutral-content shadow-base-content mb-8 flex-col rounded-md border p-6 shadow-sm'}
        >
          <div className={'w-full flex-col gap-2'}>
            <SectionHeader
              heading={'Extra Information'}
              headingClassName={'!text-xl !leading-5 !font-normal'}
              message={'Extra details for this location.'}
            />
            <div className={'mt-4 grid grid-cols-1 justify-between gap-4 md:grid-cols-3'}>
              <CardDetail header={'Scheduled Work'} data={'Coming Soon'} copyable={true} dataClassName={'truncate'} />
              <CardDetail header={'Active Notes'} data={'Coming Soon'} copyable={true} dataClassName={'truncate'} />
              <CardDetail header={'Active Clients'} data={numberOfNodes} />
              <CardDetail header={'Country ID'} data={loc.countryId == 1 ? 'United States of America' : 'Atlantis'} />
              <CardDetail header={'Created At'} data={loc.createdAt ? loc.createdAt.toString() : ''} />
              <CardDetail header={'Updated At'} data={loc.updatedAt ? loc.updatedAt.toString() : ''} />
              <CardDetail header={'Deleted At'} data={loc.deletedAt ? loc.deletedAt.toString() : ''} />
            </div>
          </div>
        </div>
      </div>
    </>
  );
}
