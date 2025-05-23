#ifndef MODEL_H
#define MODEL_H

#include "hotelapi.h"

#include <QObject>
#include<QDateTime>

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

    void initialize();

    void queryRooms(const QMap<QString, QString>& params);
    void queryGuests(const QMap<QString, QString>& params);
    void queryReserves(const QMap<QString, QString>& params);

    api::MapRooms* getRooms();
    api::MapGuests* getGuests();
    api::MapReserves* getReserves();

signals:
    void initializationFinished(bool success);
    void queryError(api::Response response);

    void roomsDataUpdated(api::MapRooms *rooms);
    void guestsDataUpdated(api::MapGuests *guests);
    void reservesDataUpdated(api::MapReserves *reserves);

private:
    api::HotelAPI api;
    api::MapRooms rooms;
    api::MapGuests guests;
    api::MapReserves reserves;

    void on_api_queryFinished(api::Response response);

};

#endif // MODEL_H
