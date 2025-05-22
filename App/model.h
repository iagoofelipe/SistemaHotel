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

    MapRooms* getRooms();
    MapGuests* getGuests();
    MapReserves* getReserves();

signals:
    void initializationFinished(bool success);
    void queryError(Response response);

    void roomsDataUpdated(MapRooms *rooms);
    void guestsDataUpdated(MapGuests *guests);
    void reservesDataUpdated(MapReserves *reserves);

private:
    HotelAPI api;
    MapRooms rooms;
    MapGuests guests;
    MapReserves reserves;

    void on_api_queryFinished(Response response);

};

#endif // MODEL_H
