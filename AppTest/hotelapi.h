#ifndef HOTELAPI_H
#define HOTELAPI_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QNetworkAccessManager>

enum EndPoint {
    GET_ROOMS, POST_ROOM, DELETE_ROOM, PUT_ROOM,
    ROOM_BY_ID,

    GET_GUEST
};

enum HttpMethod {
    GET, POST, UPDATE, DELETE, PUT
};

struct EndPointProp {
    EndPoint id;
    QString url;
    HttpMethod method;
};

struct HttpResponse {
    QString error;
    int code;
    bool success;
};

struct Room {
    QString id;
    QString name;
    int floor;
    double nightVal;
    double hourVal;
    int numCoupleBed;
    int numSingleBed;
    bool bathtub;
    bool balcony;
};

class HotelAPI : public QObject
{
    Q_OBJECT
public:
    explicit HotelAPI(QObject *parent = nullptr);
    ~HotelAPI();

    void queryRooms(std::map<QString, QVariant> *params = nullptr);
    void clear();

signals:
    void updateRoomsRequired();
    void queryRoomsFinished(HttpResponse response, QMap<QString, Room*> *rooms);

private:
    const static QString baseUrl;
    static std::map<EndPoint, EndPointProp> endpoints;

    QNetworkAccessManager manager;
    QMap<QString, Room*> rooms;

    void query(EndPoint endpoint, std::map<QString, QVariant> *params);
    void queryFinished(EndPointProp *prop, QNetworkReply *reply);

};

#endif // HOTELAPI_H
