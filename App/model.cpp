#include "model.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

Model::Model(QObject *parent)
    : QObject{parent}
    , api{this}
{
    connect(&api, &HotelAPI::initializationFinished, this, &Model::initializationFinished);
    connect(&api, &HotelAPI::queryFinished, this, &Model::on_api_queryFinished);
}

void Model::initialize() {
    api.initialize();
}

void Model::queryRooms(const QMap<QString, QString>& params) {
    api.query(GET_ROOMS, params);
}

void Model::queryGuests(const QMap<QString, QString>& params) {
    api.query(GET_GUESTS, params);
}

void Model::queryReserves(const QMap<QString, QString>& params) {
    api.query(GET_RESERVES, params);
}

void Model::on_api_queryFinished(Response response) {
    QJsonDocument jdoc = QJsonDocument::fromJson(response.data);

    // em caso de erro na consulta
    if(!response.success) {

        // caso haja uma mensagem de erro do servidor
        if(jdoc.isObject()) {
            QJsonObject jobj = jdoc.object();
            response.error = jobj["error"].toString();
        }

        emit queryError(response);
        return;
    }

    // em caso de sucesso na consulta
    const QJsonArray array = jdoc.array();
    switch (response.endpoint) {

    case GET_ROOMS:
        api.parseRoomsFromJson(rooms, array);
        emit roomsDataUpdated(&rooms);
        break;

    case GET_GUESTS:
        api.parseGuestsFromJson(guests, array);
        emit guestsDataUpdated(&guests);
        break;

    case GET_RESERVES:
        api.parseReservesFromJson(reserves, array);
        emit reservesDataUpdated(&reserves);
        break;
    }
}
