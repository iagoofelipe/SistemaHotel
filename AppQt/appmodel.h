#ifndef APPMODEL_H
#define APPMODEL_H

#include "hotelapi.h"

#include <QObject>
#include <QMap>

class AppModel : public QObject
{
    Q_OBJECT
public:
    explicit AppModel(QObject *parent = nullptr);
    ~AppModel();

    api::MapRooms* getRooms();
    api::MapGuests* getGuests();
    api::MapReserves* getReserves();
    api::HotelAPI* getAPI();

    void initialize();
    // void getRoomById(const QString& id);
    // void getGuestById(const QString& id);
    // void getReserveById(const QString& id);
    void requestGetRooms(const QMap<QString, QString>& params = {});
    void requestGetGuests(const QMap<QString, QString>& params = {});
    void requestGetReserves(const QMap<QString, QString>& params = {});

    api::Room* getRoomByIndex(int index);
    api::Guest* getGuestByIndex(int index);
    api::Reserve* getReserveByIndex(int index);

signals:
    void initializationFinished(bool api);
    void requestError(const api::Response& response);
    void dataReadyGetRooms(const api::Response& response, api::MapRooms *rooms);
    void dataReadyGetGuests(const api::Response& response, api::MapGuests *guests);
    void dataReadyGetReserves(const api::Response& response, api::MapReserves *reserves);

private:
    api::HotelAPI *api;
    api::MapRooms rooms;
    api::MapGuests guests;
    api::MapReserves reserves;

    void on_api_requestFinished(api::Response response);
    void on_api_initializationFinished(bool success);
};

#endif // APPMODEL_H
