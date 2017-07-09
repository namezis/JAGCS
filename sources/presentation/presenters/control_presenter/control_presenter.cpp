#include "control_presenter.h"

// Qt
#include <QMap>
#include <QVariant>
#include <QDebug>

// Internal
#include "domain_entry.h"

#include "db_facade.h"
#include "vehicle.h"

#include "telemetry_service.h"

#include "vehicle_type_mapper.h"
#include "location_map_presenter.h"
#include "video_split_presenter.h"

#include "dashboard_presenter.h"
#include "aerial_dashboard_factory.h"

using namespace presentation;

class ControlPresenter::Impl
{
public:
    domain::DomainEntry* entry;

    AbstractMapPresenter* map;
    VideoSplitPresenter* video;
    DashboardPresenter* dashboard = nullptr;
};

ControlPresenter::ControlPresenter(domain::DomainEntry* entry, QObject* parent):
    BasePresenter(parent),
    d(new Impl())
{
    d->entry = entry;

    db::DbFacade* dbFacade = entry->dbFacade();

    d->map = new LocationMapPresenter(entry, this);
    d->video = new VideoSplitPresenter(dbFacade, this);

    connect(dbFacade, &db::DbFacade::vehicleAdded, this, &ControlPresenter::updateVehiclesList);
    connect(dbFacade, &db::DbFacade::vehicleRemoved, this, &ControlPresenter::updateVehiclesList);
    connect(dbFacade, &db::DbFacade::vehicleChanged, this, &ControlPresenter::updateVehiclesList);
}

ControlPresenter::~ControlPresenter()
{
    if (d->map->view()) d->map->saveViewPort();
}

void ControlPresenter::updateVehiclesList()
{
    QStringList vehicles;
    vehicles.append(QString());

    for (const db::VehiclePtr& vehicle: d->entry->dbFacade()->vehicles())
    {
        vehicles.append(vehicle->name());
    }

    this->setViewProperty(PROPERTY(vehicles), vehicles);
}

void ControlPresenter::connectView(QObject* view)
{
    d->map->setView(view->findChild<QObject*>(NAME(map)));
    d->video->setView(view->findChild<QObject*>(NAME(video)));

    connect(view, SIGNAL(selectVehicle(int)), this, SLOT(onSelectVehicle(int)));

    this->updateVehiclesList();
}

void ControlPresenter::onSelectVehicle(int index)
{
    // TODO: check, if vehicle is the same
    db::VehiclePtrList vehicles  = d->entry->dbFacade()->vehicles();

    if (d->dashboard) delete d->dashboard;

    if (index > 0 && index <= vehicles.count())
    {
        db::VehiclePtr vehicle = vehicles[index - 1];

        switch (vehicle->type()) {
        case db::Vehicle::FixedWing:
        case db::Vehicle::FlyingWing:
        case db::Vehicle::Quadcopter:
        case db::Vehicle::Hexcopter:
        case db::Vehicle::Octocopter:
        case db::Vehicle::Helicopter:
        case db::Vehicle::Coaxial:
        case db::Vehicle::Vtol:
        case db::Vehicle::Airship:
        case db::Vehicle::Kite:
        case db::Vehicle::Ornithopter: {
            AerialDashboardFactory factory(d->entry, vehicle);
            d->dashboard = factory.create();
            break;
        }
        default:  {
            GenericDashboardFactory factory(d->entry, vehicle);
            d->dashboard = factory.create();
            break;
        }
        }

        if (d->dashboard)
        {
            d->dashboard->setParent(this);
            d->dashboard->setView(this->view()->findChild<QObject*>(NAME(dashboard)));
            d->dashboard->setViewProperty(PROPERTY(vehicleMark), ::vehicleIcon(vehicle->type()));
        }
    }
    else
    {
        d->dashboard = nullptr;
    }
}


