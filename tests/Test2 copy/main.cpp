#include <QCoreApplication>

#include "hotelapi.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    HotelAPI api;

    QMap<QString, QString> params {
        { "numCoupleBed" , "1" }
        // { "numCoupleBed" , "3" }
    };

    // api.queryRooms();
    api.query(EndPoint::GET_ROOMS, &params);
    // api.query(GET_GUESTS);
    // api.query(GET_RESERVES);

    return a.exec();
}
