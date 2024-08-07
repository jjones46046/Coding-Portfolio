import type { ColumnDef } from '@components/DataTable';
import type { XIQClient } from '@services/xiq.server';

import { DataTable } from '@components/DataTable';
import { SectionHeader } from '@components/SectionHeader';
import { useOdinContext } from '@context/OdinProvider';

export function TabClients({
  xiq_clients,
  clientColumns
}: {
  xiq_clients: XIQClient;
  clientColumns: ColumnDef<(typeof xiq_clients)[0]>[];
}) {
  const { toggleFlyout } = useOdinContext();
  return (
    <>
      <div role="tabpanel" className="tab-content w-full py-10">
        <div
          id={'location-clients'}
          className={'border-neutral-content shadow-base-content mb-8 flex-col rounded-md border p-6 shadow-sm'}
        >
          <div className={'w-full flex-col'}>
            <SectionHeader
              heading={'Clients'}
              headingClassName={'!text-xl !leading-5 !font-normal'}
              message={
                'Listing of clients that are showing as connected to one or more online device in this location.'
              }
            />
            <DataTable
              data={xiq_clients}
              columns={clientColumns}
              options={{ actionFunction: (data) => toggleFlyout({ area: 'client_detail', data }) }}
            />
          </div>
        </div>
      </div>
    </>
  );
}
