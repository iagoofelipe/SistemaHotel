#ifndef HOTELAPI_H
#define HOTELAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QMap>
#include <QVariant>
#include <QStringList>

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
    QString guestId;
    QString checkIn;
    QString checkOut;
};

typedef QMap<QString, Room> MapRooms;
typedef QMap<QString, Guest> MapGuests;
typedef QMap<QString, Reserve> MapReserves;

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

    // ------------------------------------------------------------------------
    // Public Static
    static const QStringList RoomTableLabels;

    static QString getUrlByEndPointId(EndPoint ep);
    static void displayDebugResponseRooms(Response response, MapRooms *rooms);
    static void displayDebugResponseGuests(Response response, MapGuests *guests);
    static void displayDebugResponseReserves(Response response, MapReserves *reserves);

    // ------------------------------------------------------------------------
    // Public Methods
    void initialize();
    void query(EndPoint ep, const QMap<QString, QString>& params);
    void create(EndPoint ep, QJsonObject json);
    void parseRoomsFromJson(MapRooms& rooms, const QJsonArray& array);
    void parseGuestsFromJson(MapGuests& guests, const QJsonArray& array);
    void parseReservesFromJson(MapReserves& reserves, const QJsonArray& array);

signals:
    void initializationFinished(bool success);
    void queryFinished(Response response);
    void createFinished(Response response);

private:
    // ------------------------------------------------------------------------
    // Private Static
    const static QString baseUrl;

    // ------------------------------------------------------------------------
    // Private Attributes
    QNetworkAccessManager manager;

    // ------------------------------------------------------------------------
    // Private Methods
    Response generateResponse(EndPoint ep, QNetworkReply *reply);
    
    // ------------------------------------------------------------------------
};

}

#endif // HOTELAPI_H
