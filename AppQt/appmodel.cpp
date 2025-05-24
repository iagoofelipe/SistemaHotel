#include "appmodel.h"

#include <QJsonDocument>

AppModel::AppModel(QObject *parent)
    : QObject{parent}
    , api(new api::HotelAPI(this))
{
    connect(api, &api::HotelAPI::requestFinished, this, &AppModel::on_api_requestFinished);
    connect(api, &api::HotelAPI::initializationFinished, this, &AppModel::on_api_initializationFinished);
}

AppModel::~AppModel() {

}

api::MapRooms* AppModel::getRooms() { return &rooms; }
api::MapGuests* AppModel::getGuests() { return &guests; }
api::MapReserves* AppModel::getReserves() { return &reserves; }
api::HotelAPI* AppModel::getAPI() { return api; }

void AppModel::requestGetRooms(const QMap<QString, QString>& params) {
    api->request(api::EndPoint::GET_ROOMS, params);
}

void AppModel::requestGetGuests(const QMap<QString, QString>& params) {
    api->request(api::EndPoint::GET_GUESTS, params);
}

void AppModel::requestGetReserves(const QMap<QString, QString>& params) {
    api->request(api::EndPoint::GET_RESERVES, params);
}

api::Room* AppModel::getRoomByIndex(int index) {
    QList<QString> keys = rooms.keys();
    if(index < 0 || index >= keys.size())
        return nullptr;

    return rooms[keys[index]];
}

api::Guest* AppModel::getGuestByIndex(int index) {
    QList<QString> keys = guests.keys();
    if(index < 0 || index >= keys.size())
        return nullptr;

    return guests[keys[index]];
}

api::Reserve* AppModel::getReserveByIndex(int index) {
    QList<QString> keys = reserves.keys();
    if(index < 0 || index >= keys.size())
        return nullptr;

    return reserves[keys[index]];
}

void AppModel::initialize() {
    api->initialize();
}

void AppModel::on_api_requestFinished(api::Response response) {
    QJsonDocument jdoc = QJsonDocument::fromJson(response.data);
    qDebug() << "requestFinished success" << response.success << response.error;

    // em caso de erro na consulta
    if(!response.success) {

        // caso haja uma mensagem de erro do servidor
        if(jdoc.isObject()) {
            QJsonObject jobj = jdoc.object();
            response.error = jobj["error"].toString();
        }

        emit requestError(response);
        return;
    }

    // em caso de sucesso na consulta
    const QJsonArray array = jdoc.array();

    switch(response.endpoint) {

    case api::GET_ROOMS:
        api::HotelAPI::parseRoomsFromJson(rooms, array);
        emit dataReadyGetRooms(response, &rooms);
        break;

    case api::GET_GUESTS:
        api::HotelAPI::parseGuestsFromJson(guests, array);
        emit dataReadyGetGuests(response, &guests);
        break;

    case api::GET_RESERVES:
        api::HotelAPI::parseReservesFromJson(reserves, array);
        emit dataReadyGetReserves(response, &reserves);
        break;

    default:
        break;
    }
}

void AppModel::on_api_initializationFinished(bool success) {
    emit initializationFinished(success);
}
