#include "hotelapi.h"

#include <QDebug>
#include <QUrlQuery>
#include <QNetworkReply>

namespace api
{

const QString HotelAPI::baseUrl { "http://127.0.0.1:8000/api" };

const QStringList HotelAPI::RoomTableHeaders {
    "Nome", "Andar", "Valor", "Camas Casal", "Camas Solteiro", "Banheira", "Varanda"
};

const QStringList HotelAPI::GuestTableHeaders { "ID", "Nome", "E-mail" };

const QStringList HotelAPI::ReserveTableHeaders { "Cliente", "Quarto", "Andar", "CheckIn", "CheckOut" };

HotelAPI::HotelAPI(QObject *parent)
    : QObject{parent}
    , manager(this)
{
}

void HotelAPI::display(const Response& response, MapRooms *rooms) {
    qDebug() << "display" << "success" << response.success;
    if(rooms)
        display(rooms);
    else
        qDebug() << "error" << response.error;
}

void HotelAPI::display(const Response& response, MapGuests *guests) {
    qDebug() << "display" << "success" << response.success;
    if(guests)
        display(guests);
    else
        qDebug() << "error" << response.error;
}

void HotelAPI::display(const Response& response, MapReserves *reserves) {
    qDebug() << "display" << "success" << response.success;
    if(response.success)
        display(reserves);
    else
        qDebug() << "error" << response.error;
}

void HotelAPI::display(MapRooms *obj) {
    if(obj)
        for(auto [key, value] : obj->asKeyValueRange())
            display(value);
}

void HotelAPI::display(Room *obj) {
    if(obj)
        qDebug()
            << "<Room id" << obj->id
            << "name" << obj->name
            << "floor" << obj->floor
            << "nightVal" << obj->nightVal
            << "hourVal" << obj->hourVal
            << "numCoupleBed" << obj->numCoupleBed
            << "numSingleBed" << obj->numSingleBed
            << "bathtub" << obj->bathtub
            << "balcony" << obj->balcony
            << ">"
            ;
}

void HotelAPI::display(MapGuests *obj) {
    if(obj)
        for(auto [key, value] : obj->asKeyValueRange())
            display(value);
}

void HotelAPI::display(Guest *obj) {
    if(obj)
        qDebug()
            << "<Guest id" << obj->id
            << "name" << obj->name
            << "email" << obj->email
            << ">"
            ;
}

void HotelAPI::display(MapReserves *obj) {
    if(obj)
        for(auto [key, value] : obj->asKeyValueRange())
            display(value);
}

void HotelAPI::display(Reserve *obj) {
    if(obj)
        qDebug()
            << "<Reserve id" << obj->id
            << "checkIn" << obj->checkIn.toString("HH:mm dd/MM/yyyy")
            << "checkOut" << obj->checkOut.toString("HH:mm dd/MM/yyyy")
            << ">"
            ;
}

void HotelAPI::parseRoomsFromJson(MapRooms& rooms, const QJsonArray& array) {
    rooms.clear();

    for(const auto& jval : array) {
        auto obj = jval.toObject();
        rooms[obj["id"].toString()] = parseRoomFromJson(obj);
    }
}

void HotelAPI::parseGuestsFromJson(MapGuests& guests, const QJsonArray& array) {
    guests.clear();

    for(const auto& jval : array) {
        auto obj = jval.toObject();
        guests[obj["id"].toString()] = parseGuestFromJson(obj);
    }
}

void HotelAPI::parseReservesFromJson(MapReserves& reserves, const QJsonArray& array) {
    reserves.clear();

    for(const auto& jval : array) {
        auto obj = jval.toObject();
        reserves[obj["id"].toString()] = parseReserveFromJson(obj);
    }
}

Room* HotelAPI::parseRoomFromJson(const QJsonObject& obj) {
    return new Room {
        obj["id"].toString(), obj["name"].toString(), obj["floor"].toInt(),
        obj["nightVal"].toDouble(), obj["hourVal"].toDouble(),
        obj["numCoupleBed"].toInt(), obj["numSingleBed"].toInt(),
        obj["bathtub"].toBool(), obj["balcony"].toBool(),
    };
}

Guest* HotelAPI::parseGuestFromJson(const QJsonObject& obj) {
    return new Guest {
        obj["id"].toString(), obj["name"].toString(), obj["email"].toString(),
    };
}

Reserve* HotelAPI::parseReserveFromJson(const QJsonObject& obj) {
    QDateTime checkIn = QDateTime::fromString(obj["checkIn"].toString(), "yyyy-MM-ddTHH:mm:ss");
    QDateTime checkOut = QDateTime::fromString(obj["checkOut"].toString(), "yyyy-MM-ddTHH:mm:ss");

    return new Reserve {
        obj["id"].toString(), obj["roomId"].toString(), obj["roomName"].toString(),
        obj["roomFloor"].toInt(), obj["guestId"].toString(), obj["guestName"].toString(),
        checkIn, checkOut,
    };
}

void HotelAPI::initialize() {
    QUrl url(baseUrl);
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

    qDebug() << "url initialize" << url;

    QObject::connect(reply, &QNetworkReply::finished, this, [=]{
        emit this->initializationFinished(reply->error() == QNetworkReply::NoError);
        reply->deleteLater();
    });
}

void HotelAPI::request(EndPoint ep, const QMap<QString, QString>& params) {
    QUrl url = getEndPointUrl(ep);

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
        emit this->requestFinished(this->generateResponse(ep, reply));
        reply->deleteLater();
    });
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

QString HotelAPI::getEndPointUrl(EndPoint ep) {
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

}
