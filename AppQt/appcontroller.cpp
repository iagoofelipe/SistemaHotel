#include "appcontroller.h"
#include "table.h"

AppController::AppController(AppModel *model, AppView *view, QObject *parent)
    : QObject{parent}
    , model(model)
    , view(view)
{
    auto tableRooms = view->getTableRooms();
    auto tableGuests = view->getTableGuests();
    auto tableReserves = view->getTableReserves();

    connect(model, &AppModel::initializationFinished, this, &AppController::on_model_initializationFinished);
    connect(model, &AppModel::dataReadyGetRooms, this, &AppController::on_model_dataReadyGetRooms);
    connect(model, &AppModel::dataReadyGetGuests, this, &AppController::on_model_dataReadyGetGuests);
    connect(model, &AppModel::dataReadyGetReserves, this, &AppController::on_model_dataReadyGetReserves);
    connect(model, &AppModel::requestError, this, &AppController::on_model_requestError);

    connect(tableRooms, &Table::updateRequired, this, &AppController::on_tableRooms_updateRequired);
    connect(tableRooms, &Table::currentRowChanged, this, &AppController::on_tableRooms_currentRowChanged);
    connect(tableGuests, &Table::updateRequired, this, &AppController::on_tableGuests_updateRequired);
    connect(tableGuests, &Table::currentRowChanged, this, &AppController::on_tableGuests_currentRowChanged);
    connect(tableReserves, &Table::updateRequired, this, &AppController::on_tableReserves_updateRequired);
    connect(tableReserves, &Table::currentRowChanged, this, &AppController::on_tableReserves_currentRowChanged);

    connect(model->getAPI(), &api::HotelAPI::requestRequired, this, &AppController::on_api_requestRequired);
}

void AppController::initialize() {
    view->initialize();
    model->initialize();
}

void AppController::on_model_initializationFinished(bool api) {
    if(!api) {
        view->showMessage("não foi possível inicializar a API");
        return;
    }

    view->showMessage("API inicializada com êxito, consultando dados...");
    model->requestGetRooms();
    model->requestGetGuests();
    model->requestGetReserves();
}

void AppController::on_model_dataReadyGetRooms(const api::Response& response, api::MapRooms *rooms) {
    api::HotelAPI::display(response, rooms);
    view->getTableRooms()->blockUpdate(false);

    if(!rooms)
        return;

    view->showMessage("inserindo dados de quartos...", 3000);
    view->setDataTableRooms(rooms);
}

void AppController::on_model_dataReadyGetGuests(const api::Response& response, api::MapGuests *guests) {
    api::HotelAPI::display(response, guests);
    view->getTableGuests()->blockUpdate(false);

    if(!guests)
        return;

    view->showMessage("inserindo dados de hóspedes...", 3000);
    view->setDataTableGuests(guests);
}

void AppController::on_model_dataReadyGetReserves(const api::Response& response, api::MapReserves *reserves) {
    api::HotelAPI::display(response, reserves);
    view->getTableReserves()->blockUpdate(false);

    if(!reserves)
        return;

    view->showMessage("inserindo dados de reservas...", 3000);
    view->setDataTableReserves(reserves);
}

void AppController::on_model_requestError(const api::Response& response) {
    view->showMessage(response.error);
    qDebug() << "error" << response.error;
}

void AppController::on_tableRooms_updateRequired() {
    model->requestGetRooms();
}

void AppController::on_tableGuests_updateRequired() {
    model->requestGetGuests();
}

void AppController::on_tableReserves_updateRequired() {
    model->requestGetReserves();
}

void AppController::on_tableRooms_currentRowChanged(int row) {
    api::Room *room = model->getRoomByIndex(row);

    if(!room) {
        qDebug() << "tableRooms unselected row";
        return;
    }

    api::HotelAPI::display(room);
}

void AppController::on_tableGuests_currentRowChanged(int row) {
    api::Guest *guest = model->getGuestByIndex(row);

    if(!guest) {
        qDebug() << "tableGuests unselected row";
        return;
    }

    api::HotelAPI::display(guest);
}

void AppController::on_tableReserves_currentRowChanged(int row) {
    api::Reserve *reserve = model->getReserveByIndex(row);

    if(!reserve) {
        qDebug() << "tableReserves unselected row";
        return;
    }

    api::HotelAPI::display(reserve);
}

void AppController::on_api_requestRequired(api::EndPoint endpoint) {
    switch (endpoint) {

    case api::EndPoint::GET_ROOMS:
        view->getTableRooms()->blockUpdate(true);
        break;

    case api::EndPoint::GET_GUESTS:
        view->getTableGuests()->blockUpdate(true);
        break;

    case api::EndPoint::GET_RESERVES:
        view->getTableReserves()->blockUpdate(true);
        break;

    default:
        break;
    }
}
