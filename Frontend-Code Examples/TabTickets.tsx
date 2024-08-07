export function TabTickets() {
  return (
    <>
      <div role="tabpanel" className="tab-content w-full py-10">
        <div
          id={'location-tickets'}
          className={'border-neutral-content shadow-base-content mb-8 flex-col rounded-md border p-6 shadow-sm'}
        >
          <div className={'w-full flex-col'}>
            <div className="mb-2 flex-row justify-between">
              <div className="w-full flex-col">
                <h2 className={'text-primary text-xl leading-5'}>Tickets</h2>
              </div>
            </div>
            <div>Tickets associated to this location.</div>
          </div>
        </div>
      </div>
    </>
  );
}
