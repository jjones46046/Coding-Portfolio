import type { SelectLocationFull } from '@fulcrum/shared/schema/fulcrum';

import { Badge } from 'react-daisyui';

import { CardDetail } from '@components/CardDetail';

export function PanelDetail({ loc }: { loc: SelectLocationFull }) {
  return (
    <>
      <div id={'locations-detail'} className={'w-full justify-self-start'}>
        <div className={'shadow-base-content mb-8 flex-col rounded-md border p-6 shadow-sm'}>
          <div className="flex-row justify-between">
            <div className="w-full flex-col">
              <h2 className={'text-primary mb-4 text-xl leading-5'}>Detailed Information</h2>
              <div className={'grid grid-cols-1 gap-4 md:grid-cols-3'}>
                <CardDetail
                  header={'Company Name'}
                  data={loc.tenant?.name}
                  copyable={true}
                  className={'md:col-span-3'}
                  dataClassName={'truncate'}
                />
                <CardDetail header={'Channel'} data={loc.channel} />
                <CardDetail header={'Location Type'} data={loc.locationTypeLabel} className={'md:col-span-2'} />
                <CardDetail
                  header={'Status'}
                  data={
                    <Badge
                      size={'md'}
                      responsive={true}
                      color={
                        loc.status == 'active' && loc.connectors[0]
                          ? 'success'
                          : loc.status == 'active' && !loc.connectors[0]
                            ? 'warning'
                            : 'error'
                      }
                    >
                      {loc.status == 'active' && loc.connectors[0]
                        ? 'ACTIVE'
                        : loc.status == 'active' && !loc.connectors[0]
                          ? 'NOT CUTOVER'
                          : 'DOWN'}
                    </Badge>
                  }
                />
                <CardDetail header={'IP Schema'} data={loc.ipSchema} />
                <CardDetail header={'Aptos Network'} data={loc.aptosNetwork} />
              </div>
            </div>
          </div>
        </div>
      </div>
    </>
  );
}
