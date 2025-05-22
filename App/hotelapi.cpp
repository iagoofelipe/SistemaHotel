#include "hotelapi.h"

#include <QCoreApplication>
#include <QDebug>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

const QString HotelAPI::baseUrl { "http://127.0.0.1:8000/api" };

HotelAPI::HotelAPI(QObject *parent)
    : QObject{parent}
{
}

const QStringList HotelAPI::RoomTableLabels {
    "Nome", "Andar", "Valor", "QTD Camas Casal", "QTD Camas Solteiro", "Banheira", "Varanda"
};

QString HotelAPI::getUrlByEndPointId(EndPoint ep) {
    QString str_url = baseUrl;

    switch (ep) {

    case GET_ROOMS:
        str_url += "/room";
        break;

    case GET_GUESTS:
        str_url += "/guest";
        break;

    case GET_RESERVES:
        str_url += "/reserve";
        break;
    }

    return str_url;
}

void HotelAPI::initialize() {
    QUrl url(baseUrl);
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

    QObject::connect(reply, &QNetworkReply::finished, this, [=]{
        reply->deleteLater();
        emit this->initializationFinished(reply->error() == QNetworkReply::NoError);
    });
}

void HotelAPI::query(EndPoint ep, const QMap<QString, QString>& params) {
    QUrl url = getUrlByEndPointId(ep);

    if(!params.empty()) {
        QUrlQuery query;

        for(auto [key, val] : params.asKeyValueRange())
            query.addQueryItem(key, val);

        url.setQuery(query);
    }

    qDebug() << "url query" << url;

    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

    QObject::connect(reply, &QNetworkReply::finished, this, [=]() {
        reply->deleteLater();
        emit queryFinished(this->generateResponse(ep, reply));
    });
}

void HotelAPI::create(EndPoint ep, QJsonObject json) {
    QUrl url(getUrlByEndPointId(ep));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // adicionando corpo da requisição
    QJsonDocument jdoc(json);
    QNetworkReply *reply = manager.post(request, jdoc.toJson());

    QObject::connect(reply, &QNetworkReply::finished, this, [=]() {
        reply->deleteLater();
        emit this->createFinished(this->generateResponse(ep, reply));
    });
}

void HotelAPI::parseRoomsFromJson(MapRooms& rooms, const QJsonArray& array) {
    rooms.clear();

    for(const auto& jval : array) {
        auto jobj = jval.toObject();
        QString id = jobj["id"].toString();

        rooms[id] = Room {
            id, jobj["name"].toString(), jobj["floor"].toInt(),
            jobj["nightVal"].toDouble(), jobj["hourVal"].toDouble(),
            jobj["numCoupleBed"].toInt(), jobj["numSingleBed"].toInt(),
            jobj["bathtub"].toBool(), jobj["balcony"].toBool(),
        };
    }
}

void HotelAPI::parseGuestsFromJson(MapGuests& guests, const QJsonArray& array) {
    guests.clear();

    for(const auto& jval : array) {
        auto jobj = jval.toObject();
        QString id = jobj["id"].toString();

        guests[id] = Guest {
            id, jobj["name"].toString(), jobj["email"].toString(),
        };
    }
}

void HotelAPI::parseReservesFromJson(MapReserves& reserves, const QJsonArray& array) {
    reserves.clear();

    for(const auto& jval : array) {
        auto jobj = jval.toObject();
        QString id = jobj["id"].toString();

        reserves[id] = Reserve {
            id, jobj["room"].toString(), jobj["guest"].toString(),
            jobj["checkIn"].toString(), jobj["checkOut"].toString(),
        };
    }
}


// MapRooms* HotelAPI::getRooms() { return &rooms; }
// MapGuests* HotelAPI::getGuests() { return &guests; }
// MapReserves* HotelAPI::getReserves() { return &reserves; }

Response HotelAPI::generateResponse(EndPoint ep, QNetworkReply *reply) {
    bool success = reply->error() == QNetworkReply::NoError;

    return Response {
        ep,
        reply->url().toString(),
        success,
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(),
        success? "" : reply->errorString(),
        reply->readAll(),
    };
}

/*
MapRooms* HotelAPI::generateRoomsFromJArray(bool success, const QJsonArray& array) {
    if(!success)
        return nullptr;

    rooms.clear();

    for(const auto& jval : array) {
        auto jobj = jval.toObject();
        QString id = jobj["id"].toString();

        rooms[id] = Room {
            id, jobj["name"].toString(), jobj["floor"].toInt(),
            jobj["nightVal"].toDouble(), jobj["hourVal"].toDouble(),
            jobj["numCoupleBed"].toInt(), jobj["numSingleBed"].toInt(),
            jobj["bathtub"].toBool(), jobj["balcony"].toBool(),
        };
    }

    return &rooms;
}

MapGuests* HotelAPI::generateGuestsFromJArray(bool success, const QJsonArray& array) {
    if(!success)
        return nullptr;

    guests.clear();

    for(const auto& jval : array) {
        auto jobj = jval.toObject();
        QString id = jobj["id"].toString();

        guests[id] = Guest {
            id, jobj["name"].toString(), jobj["email"].toString(),
        };
    }

    return &guests;
}

MapReserves* HotelAPI::generateReservesFromJArray(bool success, const QJsonArray& array) {
    if(!success)
        return nullptr;

    reserves.clear();

    for(const auto& jval : array) {
        auto jobj = jval.toObject();
        QString id = jobj["id"].toString();

        reserves[id] = Reserve {
            id, jobj["room"].toString(), jobj["guest"].toString(),
            jobj["checkIn"].toString(), jobj["checkOut"].toString(),
        };
    }

    return &reserves;
}
*/

// void HotelAPI::on_queryFinished(EndPoint ep, QNetworkReply *reply) {
//     qDebug() << "on_queryFinished";
//     reply->deleteLater();

//     Response response = generateResponse(ep, reply);
//     emit queryDataReady(ep, response, reply->readAll());

//     // QJsonDocument jdoc = QJsonDocument::fromJson(reply->readAll());
//     // const QJsonArray array = jdoc.array();

//     // // em caso de erro na consulta e haja uma mensagem de erro do servidor
//     // if(!response.success && jdoc.isObject()) {
//     //     QJsonObject jobj = jdoc.object();
//     //     response.error = jobj["error"].toString();
//     // }

//     // switch (ep) {

//     // case GET_ROOMS:
//     //     emit roomsDataReady(response, generateRoomsFromJArray(response.success, array));
//     //     break;

//     // case GET_GUESTS:
//     //     emit guestsDataReady(response, generateGuestsFromJArray(response.success, array));
//     //     break;

//     // case GET_RESERVES:
//     //     emit reservesDataReady(response, generateReservesFromJArray(response.success, array));
//     //     break;

//     // default:
//     //     break;
//     // }
// }

// void HotelAPI::on_createFinished(EndPoint ep, QNetworkReply *reply) {
//     qDebug() << "on_createFinished";
//     reply->deleteLater();

//     bool success = reply->error() == QNetworkReply::NoError;

//     // if(success)
//     //     query(ep);

//     emit createFinished(success);
// }

// void HotelAPI::on_roomsDataReady(Response response, MapRooms *rooms) {
//     qDebug() << "on_roomsDataReady" << "success" << response.success;

//     if(rooms)
//         for(auto [key, value] : rooms->asKeyValueRange()) {
//             qDebug()
//             << "<Room id" << value.id
//             << "name" << value.name
//             << "floor" << value.floor
//             << "nightVal" << value.nightVal
//             << "hourVal" << value.hourVal
//             << "numCoupleBed" << value.numCoupleBed
//             << "numSingleBed" << value.numSingleBed
//             << "bathtub" << value.bathtub
//             << "balcony" << value.balcony
//             << ">"
//                 ;
//         }
//     else
//         qDebug() << "error" << response.error;

//     QCoreApplication::quit();
// }

// void HotelAPI::on_guestsDataReady(Response response, MapGuests *guests) {
//     qDebug() << "on_guestsDataReady" << "success" << response.success;

//     if(guests)
//         for(auto [key, value] : guests->asKeyValueRange()) {
//             qDebug()
//             << "<Guest id" << value.id
//             << "name" << value.name
//             << "email" << value.email
//             << ">"
//                 ;
//         }
//     else
//         qDebug() << "error" << response.error;

//     QCoreApplication::quit();
// }

// void HotelAPI::on_reservesDataReady(Response response, MapReserves *reserves) {
//     qDebug() << "on_reservesDataReady" << "success" << response.success;

//     if(reserves)
//         for(auto [key, value] : reserves->asKeyValueRange()) {
//             qDebug()
//             << "<Reserve id" << value.id
//             << "checkIn" << value.checkIn
//             << "checkOut" << value.checkOut
//             << ">"
//                 ;
//         }
//     else
//         qDebug() << "error" << response.error;

//     QCoreApplication::quit();
// }
