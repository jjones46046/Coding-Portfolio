import type { ColumnDef } from '@components/DataTable';
import type { SelectLocationFull } from '@fulcrum/shared/schema/fulcrum';
import type { LoaderFunctionArgs, MetaArgs, MetaFunction } from '@remix-run/cloudflare';
import type { XIQClient } from '@services/xiq.server';
import type { ActionFunctionArgs } from 'react-router-dom';

import { Badge } from 'react-daisyui';
import { Link } from 'react-router-dom';
import { typedjson, useTypedLoaderData } from 'remix-typedjson';
import { connectors } from '@fulcrum/shared/schema/fulcrum';
import { authXIQ } from '@fulcrum/shared/utils/xiq';

import { TabReservations } from '@app/routes/_admin.$tenantId.locations.$code/TabReservations';
import nabadge from '@components/na-badge';
import { SectionHeader } from '@components/SectionHeader';
import { siteConfig } from '@config/site-config';
import { useOdinContext } from '@context/OdinProvider';
import { getDB } from '@services/db.server';
import { getXIQClients } from '@services/xiq.server';

import { PanelDetail } from './PanelDetail';
import { PanelGeo } from './PanelGeo';
import { PanelOverview } from './PanelOverview';
// import { TabActivity } from './TabActivity';
import { TabClients } from './TabClients';
// import { TabTickets } from './TabTickets';
import { TabConnectors } from './TabConnectors';
import { TabInformation } from './TabInformation';
import { TabNodes } from './TabNodes';
import { createReservation } from './action_functions/reservations';
import { deleteReservation } from './action_functions/deleteReservation';
import { cleanseBrokenSite } from './action_functions/cleanse_connector';
import { createMagicWanSite } from './action_functions/connector';
import { createReservationNew } from './action_functions/new_reservations';


export const loader = async ({ context, params }: LoaderFunctionArgs) => {
  const db = getDB(context.env);
  let loc: SelectLocationFull | undefined;
  let connectorsList: (typeof connectors.$inferSelect)[];
  let xiqClients: XIQClient = [];

  if (params.code && params.code.trim().length === 4) {
    loc = await db.query.locations.findFirst({
      where: (location, { eq }) => eq(location.code, params.code),
      with: {
        connectors: true,
        nodes: true,
        tenant: true,
        mwanSites: {
          with: {
            mwanSiteReservations: true
          }
        }
      }
    });

    if (!loc) {
      throw new Response(
        `No locations are defined or the specific location requested (${params.code}) does not exist.`,
        {
          status: 400
        }
      );
    }
  } else {
    loc = await db.query.locations.findFirst({
      where: (location, { eq }) => eq(location.id, params.code),
      with: {
        connectors: true,
        nodes: true,
        tenant: true,
        mwanSites: {
          with: {
            mwanSiteReservations: true
          }
        }
      }
    });

    if (!loc) {
      throw new Response(
        `No locations are defined or the specific location requested (${params.code}) does not exist.`,
        {
          status: 400
        }
      );
    }
  }
  connectorsList = await db.select().from(connectors);

  if (loc.nodes.length > 0) {
    // Authenticate against ExtremeAPI
    const access_token = await authXIQ({
      username: context.env.EXTREMEAPI_USER,
      password: context.env.EXTREMEAPI_PASS,
      kv: context.env.FULCRUM_KV
    });
    if (!access_token) throw new Response('Error authenticating to XIQ', { status: 500 });

    xiqClients = await getXIQClients({
      token: access_token,
      node_ids: loc.nodes.map((n) => String(n.xiq_nodeId) ?? '').filter((n) => n)
    });
  }

  return typedjson({ loc, xiqClients, connectorsList });
};

// eslint-disable-next-line unused-imports/no-unused-vars
export const action = async ({ request, params, context }: ActionFunctionArgs) => {
    const body = await request.formData();
    const flag = body.get('actionFlag') as string;
    let action = body.get('action') as string;
    let result: string = '';

    switch (flag) {
      case 'sites': {
        if (action === 'create') {
          await createMagicWanSite(context.env, body.get('locUUID') as string, body.get('serial') as string);
        }
        else if (action === 'cleanse') {
          await cleanseBrokenSite(context.env, body.get('locUUID') as string);
        }
        break;
      }
      case 'reservations': {
        if (action === 'create') {
          await createReservation(context.env, body.get('locUUID') as string, body.get('mac') as string, body.get('deviceType') as string);
        }
        else if (action === 'edit') {
          await deleteReservation(context.env, body.get('locUUID') as string, body.get('reservationId') as string);
          await createReservation(context.env, body.get('locUUID') as string, body.get('newMac') as string, body.get('deviceType') as string);
        }
        else if (action === 'delete') {
          await deleteReservation(context.env, body.get('locUUID') as string, body.get('reservationId') as string);
        }
        break;
      }
      default: {
        result = 'Invalid action';
        break;
    }
  }

  return new Response(result, { status: 200 });
};

export const meta: MetaFunction<typeof loader> = ({ data }: MetaArgs) => {
  const loc = data.loc;
  if (loc) {
    return [{ title: `${siteConfig.appName} | ${loc.code} ${loc.name}` }];
  } else {
    return [{ title: 'Uh Oh!' }];
  }
};

export default function LocationDetail() {
  const { loc, xiqClients, connectorsList } = useTypedLoaderData<typeof loader>();
  const { currTenant } = useOdinContext();

  // Need the location
  if (!loc) {
    return [{ title: 'Uh Oh!' }];
  }

  const locationLink = (location: any) => {
    if (location.trim().length > 4) {
      return <span>{location}</span>;
    } else {
      return (
        <Link to={`/${currTenant}/locations/${location}`} className="text-blue-500 hover:underline">
          {location}
        </Link>
      );
    }
  };

  const statusBadge = (a: any) => {
    return (
      <>
        <Badge size={'md'} responsive={true} color={a == 'Active' ? 'success' : a == 'Inactive' ? 'warning' : 'error'}>
          {a}
        </Badge>
      </>
    );
  };

  // const statusBadge = (a: any) => {
  //   return (
  //     <>
  //       <Badge size={'md'} responsive={true} color={a ? 'success' : 'error'}>
  //         {a ? 'ACTIVE' : 'INACTIVE'}
  //       </Badge>
  //     </>
  //   );
  // };


  // const xiqClientsColumns: ColumnDef<(typeof xiqClients)[0]>[] = [
  //   {
  //     cell: {
  //       key: 'id'
  //     },
  //     header: {
  //       text: 'ID'
  //     }
  //   },
  //   {
  //     cell: {
  //       key: 'mac_address'
  //     },
  //     header: {
  //       text: 'MAC Address'
  //     }
  //   },
  //   {
  //     cell: {
  //       key: 'hostname'
  //     },
  //     header: {
  //       text: 'Hostame'
  //     }
  //   },
  //   {
  //     cell: {
  //       key: 'device_name'
  //     },
  //     header: {
  //       text: 'Network Device'
  //     }
  //   },
  //   {
  //     cell: {
  //       key: 'ip_address',
  //       html: nabadge
  //     },
  //     header: {
  //       text: 'IP Address'
  //     }
  //   },
  //   {
  //     cell: {
  //       key: 'vlan'
  //     },
  //     header: {
  //       text: 'Connected VLAN'
  //     }
  //   }
  // ];

  const reservationsColumns: ColumnDef<(typeof loc.mwanSites.mwanSiteReservations)[0]>[] = loc.mwanSites
    ? [
      {
        cell: {
          key: 'id'
        },
        header: {
          text: 'ID'
        }
      },
      {
        cell: {
          key: 'name'
        },
        header: {
          text: 'Name'
        }
      },
      {
        cell: {
          key: 'reservationsMac'
        },
        header: {
          text: 'MAC Address'
        }
      },
      {
        cell: {
          key: 'reservationsIp'
        },
        header: {
          text: 'IP'
        }
      },
      {
        cell: {
          key: 'reservationsType'
        },
        header: {
          text: 'Type'
        }
      }
    ]
    : [];

  const nodesColumns: ColumnDef<(typeof loc.nodes)[0]>[] = [
    {
      cell: {
        key: 'id'
      },
      header: {
        text: 'ID'
      }
    },
    {
      cell: {
        key: 'name'
      },
      header: {
        text: 'Name'
      }
    },
    {
      cell: {
        key: 'kind'
      },
      header: {
        text: 'Kind'
      }
    },
    {
      cell: {
        key: 'model',
        html: nabadge
      },
      header: {
        text: 'Model'
      }
    },
    {
      cell: {
        key: 'lastSeen',
        html: nabadge
      },
      header: {
        text: 'Last Seen'
      }
    }
  ];

  const connectorColumns: ColumnDef<(typeof connectorsList)[0]>[] = [
    {
      cell: {
        key: 'id'
      },
      header: {
        text: 'ID'
      }
    },
    {
      cell: {
        key: 'serial'
      },
      header: {
        text: 'Serial'
      }
    },
    {
      cell: {
        key: 'activeDevice',
        html: statusBadge
      },
      header: {
        text: 'Status'
      }
    },
    {
      cell: {
        key: 'locationCode',
        html: locationLink
      },
      header: {
        text: 'Connected To'
      }
    }
  ];

  const clientColumns: ColumnDef<(typeof xiqClients)[0]>[] = [
    {
      cell: {
        key: 'id'
      },
      header: {
        text: 'ID'
      }
    },
    {
      cell: {
        key: 'hostname',
        html: nabadge
      },
      header: {
        text: 'Hostname'
      }
    },
    {
      cell: {
        key: 'device_name'
      },
      header: {
        text: 'Network Device'
      }
    },
    {
      cell: {
        key: 'mac_address'
      },
      header: {
        text: 'MAC Address'
      }
    },
    {
      cell: {
        key: 'ip_address',
        html: nabadge
      },
      header: {
        text: 'IP Address'
      }
    },
    {
      cell: {
        key: 'vlan'
      },
      header: {
        text: 'Connected VLAN'
      }
    },
    {
      cell: {
        key: 'ssid',
        html: nabadge
      },
      header: {
        text: 'Connected SSID'
      }
    }
  ];

  // const center: google.maps.LatLngLiteral = { lat: loc.latitude ?? 0, lng: loc.longitude ?? 0 };
  // const zoom = 14;

  // mapsLoader.importLibrary('maps').then(() => {
  //   map = new google.maps.Map(document.getElementById('map') as HTMLElement, {
  //     center,
  //     zoom
  //   });
  // });

  return (
    <>
      <SectionHeader
        heading={`${loc.code} ${loc.name}`}
        linkText={'Back to Locations'}
        linkPath={`/${currTenant}/locations`}
      />
      <section className={'w-full'}>
        <div
          id={'location-overview'}
          className={'border-neutral-content shadow-base-content mb-8 flex-col rounded-md border p-6 shadow-sm'}
        >
          <PanelOverview loc={loc} />
        </div>
        <div className={'mt-5 grid grid-cols-1 gap-6 xl:grid-cols-2'}>
          <PanelDetail loc={loc} />
          <PanelGeo loc={loc} />
        </div>
        <div id={'locations-tabs'}>
          <div role="tablist" className="tabs tabs-bordered tabs-lg">
            <input type="radio" name="loc_tabs_1" role="tab" className="tab" aria-label="Information" />
            <TabInformation loc={loc} xiq_clients={xiqClients} />

            <input type="radio" name="loc_tabs_1" role="tab" className="tab" aria-label="Nodes" defaultChecked={true} />
            <TabNodes loc_nodes={loc.nodes} nodesColumns={nodesColumns} />

            <input type="radio" name="loc_tabs_1" role="tab" className="tab" aria-label="Clients" />
            <TabClients xiq_clients={xiqClients} clientColumns={clientColumns} />

            {/* <input type="radio" name="loc_tabs_1" role="tab" className="tab" aria-label="Tickets" />
            <TabTickets /> */}

            {/* <input type="radio" name="loc_tabs_1" role="tab" className="tab" aria-label="Activity" />
            <TabActivity /> */}

            <input type="radio" name="loc_tabs_1" role="tab" className="tab" aria-label="Connectors" />
            <TabConnectors
              connectors_list={connectorsList}
              connectorColumns={connectorColumns}
              loc={loc}
              loc_connector={loc.connectors[0]}
              loc_site={loc.mwanSites}
            />

            {loc.mwanSites && (
              <>
                <input type="radio" name="loc_tabs_1" role="tab" className="tab" aria-label="Reservations" />
                <TabReservations
                  mwanSites={loc.mwanSites}
                  reservationsColumns={reservationsColumns}
                  loc={loc}
                />
              </>
            )}
          </div>
        </div>
      </section>
    </>
  );
}
