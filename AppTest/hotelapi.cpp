#include "hotelapi.h"

#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

const QString HotelAPI::baseUrl { "http://127.0.0.1:8000/api" };

std::map<EndPoint, EndPointProp> HotelAPI::endpoints {
    { EndPoint::GET_ROOMS, { EndPoint::GET_ROOMS, "/room", HttpMethod::GET } },
    { EndPoint::POST_ROOM, { EndPoint::POST_ROOM, "/room", HttpMethod::POST } },
    { EndPoint::DELETE_ROOM, { EndPoint::DELETE_ROOM, "/room", HttpMethod::DELETE } },
    { EndPoint::PUT_ROOM, { EndPoint::PUT_ROOM, "/room", HttpMethod::PUT } },
    { EndPoint::ROOM_BY_ID, { EndPoint::ROOM_BY_ID, "/getRoomById", HttpMethod::GET } },

    { EndPoint::GET_GUEST, { EndPoint::GET_GUEST, "/guest", HttpMethod::GET } },
};

HotelAPI::HotelAPI(QObject *parent)
    : QObject{parent}
{}

HotelAPI::~HotelAPI() {
    clear();
}

void HotelAPI::queryRooms(std::map<QString, QVariant> *params) {
    query(GET_ROOMS, params);
}

void HotelAPI::clear() {
    // rooms
    for(auto [k, v] : rooms.asKeyValueRange())
        delete v;
    rooms.clear();

}

void HotelAPI::queryFinished(EndPointProp *prop, QNetworkReply *reply) {
    bool success = reply->error() == QNetworkReply::NoError;

    HttpResponse response {
        "",
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(),
        success,
    };

    // convertendo em JSON
    QJsonDocument jdoc = QJsonDocument::fromJson(reply->readAll());

    if(jdoc.isNull()) {
        response.error = "couldn't read the json response";

    // caso retorne uma mensagem de erro
    } else if(!success) {
        auto obj = jdoc.object();
        response.error = obj["error"].toString();

    }

    reply->deleteLater();

    switch (prop->id) {
    case GET_ROOMS:

        if(success) {
            // limpando cache
            for(auto [k, v] : rooms.asKeyValueRange()) {
                delete v;
            }

            rooms.clear();

            const QJsonArray array = jdoc.array();

            for(const QJsonValue& obj : array) {
                QString id = obj["id"].toString();

                rooms[id] = new Room {
                    id, obj["name"].toString(), obj["floor"].toInt(),
                    obj["nightVal"].toDouble(), obj["hourVal"].toDouble(),
                    obj["numCoupleBed"].toInt(), obj["numSingleBed"].toInt(),
                    obj["bathtub"].toBool(), obj["balcony"].toBool(),
                };
            }

        }

        emit queryRoomsFinished(response, success? &rooms : nullptr);
        break;

    default:
        break;
    }
}

void HotelAPI::query(EndPoint endpoint, std::map<QString, QVariant> *params) {
    emit updateRoomsRequired();

    EndPointProp *prop = &endpoints[endpoint];
    QUrl url(baseUrl + prop->url);
    QNetworkRequest request(url);
    QNetworkReply *reply = nullptr;



    switch (prop->method) {
    case HttpMethod::GET:
        qDebug() << "GET" << prop->url;
        reply = manager.get(request);
        break;

    default:
        break;
    }

    if(!reply)
        return;

    QObject::connect(reply, &QNetworkReply::finished, [&]() {
        this->queryFinished(prop, reply);
    });
}
