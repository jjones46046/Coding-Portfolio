import type { ColumnDef } from '@components/DataTable';
import type { connectors, mwan_sites, SelectLocationFull } from '@fulcrum/shared/schema/fulcrum';

import { useState } from 'react';
import { Badge, Form } from 'react-daisyui';

import { CardDetail } from '@components/CardDetail';
import { DataTable } from '@components/DataTable';
import { SectionHeader } from '@components/SectionHeader';

export function TabConnectors({
  connectors_list,
  connectorColumns,
  loc,
  loc_connector,
  loc_site
}: {
  connectors_list: (typeof connectors.$inferSelect)[];
  connectorColumns: ColumnDef<(typeof connectors_list)[0]>[];
  loc: SelectLocationFull;
  loc_connector: typeof connectors.$inferSelect | undefined;
  loc_site: typeof mwan_sites.$inferSelect;
}) {
  const [showAddConnectorTab, setShowAddConnectorTab] = useState(false);
  const [selectedConnector, setSelectedConnector] = useState<string | null>(null);
  const [successMessage, setSuccessMessage] = useState<string | null>(null);
  const [invalidSelection, setInvalidSelection] = useState<boolean>(false);

  const handleAddConnectorClick = () => {
    setShowAddConnectorTab(true);
  };

  const handleCheck = (id: string, value0: string, value1: string, value2: string) => {
    setSelectedConnector(value1);
    validateSelection(value2);
  };

  const validateSelection = (activeStatus: string) => {
    if (activeStatus === 'Active') {
      setInvalidSelection(true); // Show error message if the connector is already active
    }
    else if (activeStatus === 'Inactive') {
      setInvalidSelection(false); // Reset error message if the connector is inactive
    }
  }

  const handleAddConnector = () => {
    setTimeout(() => {
      // Show success message after the backend call
      setSuccessMessage(
        `Successfully added connector ${selectedConnector} to location ${loc.code}. Connector may take a few minutes to appear active.`
      );
    }, 2000); // Simulated delay of 2 seconds
  };

  return (
    <>
      <div role="tabpanel" className="tab-content w-full py-10">
        {loc_connector ? (
          <div
            id="location-extra-info"
            className="border-neutral-content shadow-base-content mb-8 rounded-md border p-6 shadow-sm"
          >
            {/* Header Section */}
            <div className="mb-6">
              <SectionHeader
                heading="Connector Information"
                headingClassName="!text-xl !leading-5 !font-normal"
                message={
                  <>
                    Details of the connector and site attached to this location.
                    <br />
                    Note: Removing the connector will destroy the Cloudflare site. 
                  </>}
              />
            </div>

            {/* Information and Button Grid */}
            <div className="grid grid-cols-1 gap-4 md:grid-cols-2 lg:grid-cols-3 items-center">
              {/* Information Details */}
              <div className="grid grid-cols-1 gap-4 md:grid-cols-2 lg:grid-cols-2 lg:col-span-2">
                <CardDetail
                  header="Connector Status"
                  data={
                    <Badge
                      size="md"
                      responsive
                      color={loc_connector.activeDevice === 'Active' ? 'success' : 'error'}
                    >
                      {loc_connector.activeDevice === 'Active' ? 'ACTIVE' : 'INACTIVE'}
                    </Badge>
                  }
                />
                <CardDetail
                  header="Connector Serial"
                  data={loc_connector.serial}
                  copyable
                  dataClassName="truncate"
                />
                <CardDetail
                  header="Connector ID"
                  data={loc_connector.id}
                  copyable
                  dataClassName="truncate"
                />
                <CardDetail
                  header="MWAN Site ID"
                  data={loc_site.mwanSiteId}
                  copyable
                  dataClassName="truncate"
                />
              </div>

              {/* Remove Connector Button */}
              <div className="flex justify-end lg:col-span-1">
                <Form method="post" onSubmit={handleAddConnector}>
                  <input type="hidden" name="actionFlag" value="sites" />
                  <input type="hidden" name="action" value="cleanse" />
                  <input type="hidden" name="locUUID" value={loc.id} />
                  <button
                    className="btn bg-red-500 hover:bg-red-600 text-white font-semibold py-2 px-4 rounded"
                    type="submit"
                  >
                    Remove Connector
                  </button>
                </Form>
              </div>
            </div>
          </div>
        ) : (
          //If there is no connector attached, show the add connector tab so the user can add a connector.
          <div
            id={'location-tickets'}
            className={'border-neutral-content shadow-base-content mb-8 flex-col rounded-md border p-6 shadow-sm'}
          >
            <div className={'w-full flex-col'}>
              <SectionHeader
                heading={'Connectors'}
                headingClassName={'!text-xl !leading-5 !font-normal'}
                message={'Add a connector to this site or view the connector/s attached.'}
              />
              {!showAddConnectorTab && (
                <div>
                  <button onClick={handleAddConnectorClick} className="btn btn-primary">
                    Add Connector
                  </button>
                </div>
              )}
              {showAddConnectorTab && !successMessage && (
                <div className={'mt-4'}>
                  <div className="flex w-2/6 items-center justify-between rounded-lg border p-4">
                    <div>
                      <h3>Selected Connector:</h3>
                      <ul>
                        <li>{selectedConnector}</li>
                      </ul>
                    </div>
                    <div>
                      {selectedConnector && !invalidSelection ? (
                        <Form method="post" onSubmit={handleAddConnector}>
                          <input type="hidden" name="serial" value={selectedConnector} />
                          <input type="hidden" name="actionFlag" value={'sites'} />
                          <input type="hidden" name="action" value={'create'} />
                          <input type="hidden" name="locUUID" value={loc.id} />
                          <button className="btn btn-primary ml-4" type="submit">
                            Add Connector
                          </button>
                        </Form>
                      ) : (
                        <button className="btn btn-primary ml-4" disabled>
                          Add Connector
                        </button>
                      )}
                    </div>
                  </div>
                  {!successMessage && (
                    <DataTable
                      data={connectors_list}
                      columns={connectorColumns}
                      options={{
                        showPagination: true,
                        showChecklist: true
                      }}
                      extHandleCheck={handleCheck}
                    />
                  )}
                </div>
              )}
              {successMessage && (
                <div
                  className="relative mt-4 rounded border border-green-400 bg-green-100 px-4 py-3 text-green-700"
                  role="alert"
                >
                  <span className="block sm:inline">{successMessage}</span>
                </div>
              )}
            </div>
          </div>
        )}
      </div>
    </>
  );
}
