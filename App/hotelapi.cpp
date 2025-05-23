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

namespace api
{

HotelAPI::HotelAPI(QObject *parent)
    : QObject{parent}
{
}

// ------------------------------------------------------------------------
// Static
const QString HotelAPI::baseUrl { "http://127.0.0.1:8000/api" };

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

    default:
        qDebug() << "undefined endpoint, using base url";
        break;
    }

    return str_url;
}

void HotelAPI::displayDebugResponseRooms(Response response, MapRooms *rooms) {
    qDebug() << "displayDebugResponseRooms" << "success" << response.success;

    if(response.success && rooms)
        for(auto [key, value] : rooms->asKeyValueRange()) {
            qDebug()
            << "<Room id" << value.id
            << "name" << value.name
            << "floor" << value.floor
            << "nightVal" << value.nightVal
            << "hourVal" << value.hourVal
            << "numCoupleBed" << value.numCoupleBed
            << "numSingleBed" << value.numSingleBed
            << "bathtub" << value.bathtub
            << "balcony" << value.balcony
            << ">"
                ;
        }
    else
        qDebug() << "error" << response.error;
}

void HotelAPI::displayDebugResponseGuests(Response response, MapGuests *guests) {
    qDebug() << "displayDebugResponseGuests" << "success" << response.success;

    if(response.success && guests)
        for(auto [key, value] : guests->asKeyValueRange()) {
            qDebug()
            << "<Guest id" << value.id
            << "name" << value.name
            << "email" << value.email
            << ">"
                ;
        }
    else
        qDebug() << "error" << response.error;
}

void HotelAPI::displayDebugResponseReserves(Response response, MapReserves *reserves) {
    qDebug() << "displayDebugResponseReserves" << "success" << response.success;

    if(response.success && reserves)
        for(auto [key, value] : reserves->asKeyValueRange()) {
            qDebug()
            << "<Reserve id" << value.id
            << "checkIn" << value.checkIn
            << "checkOut" << value.checkOut
            << ">"
                ;
        }
    else
        qDebug() << "error" << response.error;
}

// ------------------------------------------------------------------------
// Public Methods
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
        emit this->queryFinished(this->generateResponse(ep, reply));
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

}
