#ifndef HOTELAPI_H
#define HOTELAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QMap>
#include <QVariant>
#include <QStringList>

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

    static const QStringList RoomTableLabels;

    static QString getUrlByEndPointId(EndPoint ep);

    void initialize();
    void query(EndPoint ep, const QMap<QString, QString>& params);
    void create(EndPoint ep, QJsonObject json);

    void parseRoomsFromJson(MapRooms& rooms, const QJsonArray& array);
    void parseGuestsFromJson(MapGuests& guests, const QJsonArray& array);
    void parseReservesFromJson(MapReserves& reserves, const QJsonArray& array);

    // MapRooms* getRooms();
    // MapGuests* getGuests();
    // MapReserves* getReserves();

signals:
    void initializationFinished(bool success);
    void queryFinished(Response response);
    void createFinished(Response response);

    // void roomsDataUpdateRequired();
    // void roomsDataReady(Response response, MapRooms *rooms);
    // void guestsDataReady(Response response, MapGuests *guests);
    // void reservesDataReady(Response response, MapReserves *reserves);

private:
    const static QString baseUrl;

    QNetworkAccessManager manager;
    // MapRooms rooms;
    // MapGuests guests;
    // MapReserves reserves;
    // QMap<EndPoint, QUrl> cacheUrlQuery;

    Response generateResponse(EndPoint ep, QNetworkReply *reply);
    // MapRooms* generateRoomsFromJArray(bool success, const QJsonArray& array);
    // MapGuests* generateGuestsFromJArray(bool success, const QJsonArray& array);
    // MapReserves* generateReservesFromJArray(bool success, const QJsonArray& array);

private slots:
    // void on_queryFinished(EndPoint ep, QNetworkReply *reply);
    // void on_createFinished(EndPoint ep, QNetworkReply *reply);
    // void on_roomsDataReady(Response response, MapRooms *rooms);
    // void on_guestsDataReady(Response response, MapGuests *guests);
    // void on_reservesDataReady(Response response, MapReserves *reserves);
};

#endif // HOTELAPI_H
