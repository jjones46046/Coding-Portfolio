import { SectionHeader } from '@components/SectionHeader';

export function TabActivity() {
  return (
    <>
      <div role="tabpanel" className="tab-content w-full py-10">
        <div
          id={'location-activity'}
          className={'border-neutral-content shadow-base-content mb-8 flex-col rounded-md border p-6 shadow-sm'}
        >
          <div className={'w-full flex-col'}>
            <SectionHeader heading={'Activity'} headingClassName={'!text-xl !leading-5 !font-normal'} />
          </div>
        </div>
      </div>
    </>
  );
}
