#ifndef HOTELAPI_H
#define HOTELAPI_H

#include <QObject>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QJsonArray>
#include <QJsonObject>

namespace api
{

enum EndPoint {
    GET_ROOMS,
    GET_GUESTS,
    GET_RESERVES,
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

struct Guest {
    QString id;
    QString name;
    QString email;
};

struct Reserve {
    QString id;
    QString roomId;
    QString roomName;
    int roomFloor;
    QString guestId;
    QString guestName;
    QDateTime checkIn;
    QDateTime checkOut;
};

typedef QMap<QString, Room*> MapRooms;
typedef QMap<QString, Guest*> MapGuests;
typedef QMap<QString, Reserve*> MapReserves;

struct Response {
    EndPoint endpoint;
    QString url;
    bool success;
    int code;
    QString error;
    QByteArray data;
};

class HotelAPI : public QObject
{
    Q_OBJECT
public:
    explicit HotelAPI(QObject *parent = nullptr);

    static const QStringList RoomTableHeaders;
    static const QStringList GuestTableHeaders;
    static const QStringList ReserveTableHeaders;

    static void display(const Response& response, MapRooms *rooms);
    static void display(const Response& response, MapGuests *guests);
    static void display(const Response& response, MapReserves *reserves);
    static void display(Room *obj);
    static void display(MapRooms *obj);
    static void display(Guest *obj);
    static void display(MapGuests *obj);
    static void display(Reserve *obj);
    static void display(MapReserves *obj);

    static void parseRoomsFromJson(MapRooms& rooms, const QJsonArray& array);
    static void parseGuestsFromJson(MapGuests& guests, const QJsonArray& array);
    static void parseReservesFromJson(MapReserves& reserves, const QJsonArray& array);

    static Room* parseRoomFromJson(const QJsonObject& obj);
    static Guest* parseGuestFromJson(const QJsonObject& obj);
    static Reserve* parseReserveFromJson(const QJsonObject& obj);

    void initialize();
    void request(EndPoint ep, const QMap<QString, QString>& params = {});

signals:
    void initializationFinished(bool success);
    void requestRequired(api::EndPoint endpoint);
    void requestFinished(api::Response response);

private:
    const static QString baseUrl;
    QNetworkAccessManager manager;

    Response generateResponse(EndPoint ep, QNetworkReply *reply);
    QString getEndPointUrl(EndPoint ep);
};

}

#endif // HOTELAPI_H
