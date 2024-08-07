import type { SelectLocationFull } from '@fulcrum/shared/schema/fulcrum';

import { CardDetail } from '@components/CardDetail';

export function PanelGeo({ loc }: { loc: SelectLocationFull }) {
  return (
    <>
      <div id={'locations-geo'} className={'w-full justify-self-end'}>
        <div className={'shadow-base-content mb-8 flex-col rounded-md border p-6 shadow-sm'}>
          <div className="flex w-full flex-col gap-4">
            <div className={'grid grid-cols-2 gap-4'}>
              <div className={'col-span-1 space-y-4'}>
                <h2 className={'text-primary mb-4 text-xl leading-5'}>Geo Details</h2>
                <CardDetail
                  header={'Address'}
                  data={
                    <address>
                      <span>{loc.addressLine1}</span>
                      <br />
                      <span>{loc.addressLine2}</span>
                      <br />
                      <span>
                        {loc.addressCity}, {loc.addressState}
                      </span>
                      <br />
                      <span>{loc.addressZipCode}</span>
                    </address>
                  }
                />
              </div>
              <div className={'col-span-1'}>
                <div id={'map'} className={'skeleton h-80 w-full'} />
              </div>
            </div>
          </div>
        </div>
      </div>
    </>
  );
}
