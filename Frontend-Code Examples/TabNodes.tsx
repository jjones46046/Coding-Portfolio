import type { ColumnDef } from '@components/DataTable';
import type { nodes } from '@fulcrum/shared/schema/fulcrum';

import { DataTable } from '@components/DataTable';
import { SectionHeader } from '@components/SectionHeader';
import { useOdinContext } from '@context/OdinProvider';

export function TabNodes({
  loc_nodes,
  nodesColumns
}: {
  loc_nodes: (typeof nodes.$inferSelect)[];
  nodesColumns: ColumnDef<(typeof loc_nodes)[0]>[];
}) {
  const { toggleFlyout } = useOdinContext();
  return (
    <>
      <div role="tabpanel" className="tab-content w-full py-10">
        <div
          id={'location-nodes'}
          className={'border-neutral-content shadow-base-content mb-8 flex-col rounded-md border p-6 shadow-sm'}
        >
          <div className={'w-full flex-col'}>
            <SectionHeader
              heading={'Nodes'}
              headingClassName={'!text-xl !leading-5 !font-normal'}
              message={'Nodes attached to this location.'}
            />
            <div className={'mt-4'}>
              <DataTable
                data={loc_nodes}
                columns={nodesColumns}
                options={{
                  showPagination: false,
                  actionFunction: (data) => toggleFlyout({ area: 'nodes', data })
                }}
              />
            </div>
          </div>
        </div>
      </div>
    </>
  );
}
