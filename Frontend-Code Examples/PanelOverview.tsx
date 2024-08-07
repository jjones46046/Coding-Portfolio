import type { SelectLocationFull } from '@fulcrum/shared/schema/fulcrum';

import { CardDetail } from '@components/CardDetail';

export function PanelOverview({ loc }: { loc: SelectLocationFull }) {
  return (
    <>
      <div className="mb-4 flex-row justify-between">
        <div className="w-full flex-col">
          <h2 className={'text-primary text-xl leading-5'}>Overview</h2>
        </div>
      </div>
      <div className="flex-row">
        <div className="grid w-full grid-cols-1 gap-4 md:grid-cols-3">
          <CardDetail header={'Code'} data={loc.code} />
          <CardDetail header={'Name'} data={loc.name} />
          <CardDetail
            header={'Email'}
            data={
              <div>
                <a
                  href={`mailto:${loc.addressEmail}`}
                  target={'_blank'}
                  rel="noreferrer"
                  className={"link link-accent after:content-['_↗']"}
                >
                  {loc.addressEmail}
                </a>
              </div>
            }
          />
          <CardDetail header={'District'} data={loc.district} />
          <CardDetail header={'Region'} data={loc.region} />
        </div>
      </div>
    </>
  );
}
