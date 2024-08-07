import type { ColumnDef } from '@components/DataTable';
import type { SelectLocationFull } from '@fulcrum/shared/schema/fulcrum';

import { useState } from 'react';
import { Form } from 'react-daisyui';
import { RiEditLine, RiAddBoxLine } from "react-icons/ri";

import { DataTable } from '@components/DataTable';
import { SectionHeader } from '@components/SectionHeader';

export function TabReservations({

  mwanSites,
  reservationsColumns,
  loc
}: {

  mwanSites: SelectLocationFull['mwanSites'];
  reservationsColumns: ColumnDef<(typeof mwanSites)['mwanSiteReservations'][0]>[];
  loc: SelectLocationFull;
}) {
  const [showEditReservationTab, setShowEditReservationTab] = useState(false);
  const [showAddReservationTab, setShowAddReservationTab] = useState(false);
  const [selectedReservation, setSelectedReservation] = useState<string | null>(null);
  const [displayReservation, setDisplayReservation] = useState<string | null>(null);
  const [successMessage, setSuccessMessage] = useState<string | null>(null);
  const [deviceType, setDeviceType] = useState('');
  const [customMac, setCustomMac] = useState('');
  const [isEditDisabled, setIsEditDisabled] = useState(true);
  const [isCreateDisabled, setIsCreateDisabled] = useState(true);
  const [isDeleteDisabled, setIsDeleteDisabled] = useState(true);

  const handleAddReservationClick = () => {
    setShowEditReservationTab(false);
    setShowAddReservationTab(true);
    setDeviceType('');
    setCustomMac('');
    setSelectedReservation(null);
    setDisplayReservation(null);
    setIsCreateDisabled(true);
    setIsEditDisabled(true);
    setIsDeleteDisabled(true);
  };

  const handleEditReservationClick = () => {
    setShowAddReservationTab(false);
    setShowEditReservationTab(true);
    setDeviceType('');
    setCustomMac('');
    setSelectedReservation(null);
    setDisplayReservation(null);
    setIsCreateDisabled(true);
    setIsEditDisabled(true);
    setIsDeleteDisabled(true);
  };
  const handleCheck = (id: string, value0: string, value1: string) => {
    setDeviceType('');
    setCustomMac('');
    setSelectedReservation(value0);
    setDisplayReservation(value1);
    setIsEditDisabled(true);
    setIsDeleteDisabled(false);
  };

  const handleAddReservation = () => {
    setTimeout(() => {
      setSuccessMessage(`Successfully added ${selectedReservation} to location ${loc.code}.`);
    }, 2000);
  };

  const handleDeleteReservation = () => {
    setTimeout(() => {
      setSuccessMessage(`Successfully deleted ${selectedReservation} from location ${loc.code}.`);
    }, 2000);
  }

  const handleDeviceTypeChange = (event: React.ChangeEvent<HTMLSelectElement>) => {
    const value = event.target.value;
    setDeviceType(value);
    validateForm(customMac, value);
  };

  const handleMacInputChange = (event: React.ChangeEvent<HTMLInputElement>) => {
    const rawValue = event.target.value;

    let value = rawValue.replace(/[^a-fA-F0-9]/g, '');

    if (value.length > 12) {
      value = value.substring(0, 12);
    }

    setCustomMac(value.toUpperCase());
    validateForm(value, deviceType);
  };

  const validateForm = (mac: string, deviceType: string) => {
    setIsEditDisabled(mac.length !== 12 || deviceType === '' || selectedReservation === null);
    setIsCreateDisabled(mac.length !== 12 || deviceType === '');
    setIsDeleteDisabled(mac.length > 0 || deviceType !== '');
  };


  return (
    <>
      <div role="tabpanel" className="tab-content w-full py-10">
        <div
          id={'location-reservations'}
          className={'border-neutral-content shadow-base-content mb-8 flex-col rounded-md border p-6 shadow-sm'}
        >
          <div className="w-full flex items-center justify-between mb-4">
            <SectionHeader
              heading={'Reservations'}
              headingClassName={'!text-xl !leading-5 !font-normal'}
              message={'View the current reservations attached. You can create, edit, or delete reservations here.'}
            />
            <div className="inline-flex rounded-md shadow-sm" role="group">
              <button
                type="button"
                onClick={handleAddReservationClick}
                className="btn inline-flex items-center px-4 py-2 text-sm font-medium bg-neutral text-base-content border border-slate-200 rounded-e-lg rounded-r-none hover:bg-gray-700 focus:z-10 focus:ring-2 focus:ring-gray-500"
              >
                <RiAddBoxLine /> <span>Create</span>
              </button>
              <button
                type="button"
                onClick={handleEditReservationClick}
                className="btn inline-flex items-center px-4 py-2 text-sm font-medium bg-neutral text-base-content border border-slate-200 rounded-e-lg rounded-l-none hover:bg-gray-700 focus:z-10 focus:ring-2 focus:ring-gray-500"
              >
                <RiEditLine /> <span>Edit</span>
              </button>
            </div>
          </div>

          {!showAddReservationTab && !showEditReservationTab && (
            <>
              <DataTable
                data={mwanSites.mwanSiteReservations}
                columns={reservationsColumns}
                extHandleCheck={handleCheck}
              />
            </>
          )}
          {/** Add Reservation Tab */}
          {showAddReservationTab && !successMessage && (
            <div className={'mt-4'}>
              <div className="flex flex-col w-3/6 rounded-lg border p-4 space-y-4">
                <div>
                  <SectionHeader
                    heading={'Create Reservation'}
                    headingClassName={'!text-xl !leading-5 !font-normal'}
                    message={
                      <>
                        Define the type and MAC address of the device you would like to create a reservation for.
                        <br />
                        Example: 78B8D6D4E1EC or 78:B8:D6:D4:E1:EC
                      </>
                    }
                    messageClassName="!text-sm !leading-5 !font-normal"
                  />
                </div>
                {successMessage && <div className="text-green-500 mt-4">{successMessage}</div>}
                <div className="flex justify-between items-center space-x-4">
                  {/* User Input Methods (MAC input + Device Type Dropdown) + Create Reservation Form */}
                  <div className="flex flex-col space-y-2 w-3/5">
                    <select
                      value={deviceType}
                      onChange={handleDeviceTypeChange}
                      className="form-select block w-full rounded-md border-gray-300 bg-neutral py-2 pl-3 pr-10 text-base-content focus:border-gray-100 focus:outline-none focus:ring-gray-100 sm:text-sm"
                    >
                      <option value="" disabled>
                        Select Device Type
                      </option>
                      <option value="printer">Printer</option>
                      <option value="zebra">Zebra</option>
                      <option value="pinpad">Pinpad</option>
                    </select>
                    <input
                      type="text"
                      placeholder="MAC Address"
                      value={customMac}
                      onChange={handleMacInputChange}
                      maxLength={17}
                      className="input input-primary input-bordered w-full rounded px-4 py-2"
                    />
                  </div>
                  <div className="w-2/5 flex justify-center">
                    <Form method="post" onSubmit={handleAddReservation} className="flex items-center justify-center">
                      <input type="hidden" name="mac" value={customMac.toUpperCase()} />
                      <input type="hidden" name="deviceType" value={deviceType} />
                      <input type="hidden" name="locUUID" value={loc.id} />
                      <input type="hidden" name="actionFlag" value={'reservations'} />
                      <input type="hidden" name="action" value={'create'} />
                      <button
                        className={`btn btn-primary text-sm ${isCreateDisabled ? 'btn-disabled' : ''}`}
                        type="submit"
                        disabled={isCreateDisabled}
                      >
                        Create Reservation
                      </button>
                    </Form>
                  </div>
                </div>
              </div>
              {!successMessage && (
                <DataTable
                  data={mwanSites.mwanSiteReservations}
                  columns={reservationsColumns}
                  extHandleCheck={handleCheck}
                />
              )}
            </div>
          )}
          {/** Edit Reservation Tab */}
          {showEditReservationTab && !successMessage && (
            <div className={'mt-4'}>
              <div className="flex flex-col w-3/6 rounded-lg border p-4 space-y-4">
                <div>
                  <SectionHeader
                    heading={'Edit/Delete Reservation'}
                    headingClassName={'!text-xl !leading-5 !font-normal'}
                    message={
                      <>
                        Select the reservation you would like to edit, then enter a MAC and choose a device type.
                        <br />
                        Or simply select a reservation to delete. 
                        <br />
                        *Note: Name and IP address are automatically generated.*
                      </>
                    }
                    messageClassName="!text-sm !leading-5 !font-normal"
                  />
                </div>
                <div className='rounded-lg w-1/5'>
                  <h3>Selected:</h3>
                  <ul>
                    <li className="text-green-700">{displayReservation}</li>
                  </ul>
                </div>
                <div className="flex justify-between items-center space-x-4">
                  {/** User input boxes (MAC input and Device Type dropdown) */}
                  <div className="flex flex-col space-y-2 w-3/5">
                    <div className="relative group">
                      <select
                        value={deviceType}
                        onChange={handleDeviceTypeChange}
                        className={`form-select block w-full rounded-md border-gray-300 bg-neutral py-2 pl-3 pr-10 text-base-content focus:border-indigo-500 focus:outline-none focus:ring-indigo-500 sm:text-sm ${!selectedReservation ? 'opacity-50 cursor-not-allowed' : ''}`}
                        disabled={!selectedReservation}
                        data-tooltip={!selectedReservation ? 'Select a reservation from the table to edit or delete.' : ''}
                      >
                        <option value="" disabled>
                          Select Device Type
                        </option>
                        <option value="printer">Printer</option>
                        <option value="zebra">Zebra</option>
                        <option value="pinpad">Pinpad</option>
                      </select>
                      {!selectedReservation && (
                        <span className="absolute hidden group-hover:block -top-10 left-1/2 transform -translate-x-1/2 bg-gray-800 text-white text-sm py-1 px-2 rounded-md whitespace-nowrap">
                          Select a reservation from the table to edit or delete.
                        </span>
                      )}
                    </div>
                    <div className="relative group">
                      <input
                        type="text"
                        placeholder="Enter new MAC address"
                        value={customMac}
                        onChange={handleMacInputChange}
                        maxLength={17}
                        className={`input input-primary text-base-content input-bordered w-full rounded px-4 py-2 ${!selectedReservation ? 'opacity-50 cursor-not-allowed' : ''}`}
                        disabled={!selectedReservation}
                      />
                    </div>
                  </div>
                  {/* Edit and Delete Forms */}
                  <div className="w-2/5 flex flex-col items-center space-y-4">
                    <Form method="post" onSubmit={handleAddReservation} className="flex items-center justify-center w-full">
                      <input type="hidden" name="newMac" value={customMac.toUpperCase()} />
                      <input type="hidden" name="reservationId" value={selectedReservation!} />
                      <input type="hidden" name="deviceType" value={deviceType} />
                      <input type="hidden" name="locUUID" value={loc.id} />
                      <input type="hidden" name="actionFlag" value={'reservations'} />
                      <input type="hidden" name="action" value={'edit'} />
                      <button
                        className={`btn btn-primary text-sm ${isEditDisabled ? 'btn-disabled' : ''} w-full`}
                        type="submit"
                        disabled={isEditDisabled}
                      >
                        Edit Reservation
                      </button>
                    </Form>
                    <Form method="post" onSubmit={handleDeleteReservation} className="flex items-center justify-center w-full">
                      <input type="hidden" name="reservationId" value={selectedReservation!} />
                      <input type="hidden" name="locUUID" value={loc.id} />
                      <input type="hidden" name="actionFlag" value={'reservations'} />
                      <input type="hidden" name="action" value={'delete'} />
                      <button
                        className={`btn text-sm ${isDeleteDisabled ? 'btn-disabled' : ''} w-full bg-red-600 text-white hover:bg-red-700 focus:ring-red-500`}
                        type="submit"
                        disabled={isDeleteDisabled}
                      >
                        Delete Reservation
                      </button>
                    </Form>
                  </div>
                </div>
              </div>
              {!successMessage && (
                <DataTable
                  data={mwanSites.mwanSiteReservations}
                  columns={reservationsColumns}
                  extHandleCheck={handleCheck}
                  options={{
                    showChecklist: true,
                  }}
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
    </>
  );

}
