#include "hotelapi.h"

#include <QCoreApplication>
#include <QDebug>

void on_updateRoomsRequired() {
    qDebug() << "udateRoomsRequired";
}

void on_queryRoomsFinished(HttpResponse response, QMap<QString, Room*> *rooms) {

    qDebug()
        << "Success" << response.success
        << "Código" << response.code
        << "ErroMsg" << response.error
        ;

    for(auto [k, v] : rooms->asKeyValueRange()) {
        qDebug()
            << "Id:" << k
            << "<Room id=" << v->id
            << "floor=" << v->floor
            << "nightVal=" << v->nightVal
            << "hourVal=" << v->hourVal
            << "numCoupleBed=" << v->numCoupleBed
            << "numSingleBed=" << v->numSingleBed
            << "bathtub=" << v->bathtub
            << "balcony=" << v->balcony
            ;
    }

    QCoreApplication::quit();
}

void test1() {

}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    HotelAPI *api = new HotelAPI;

    QObject::connect(api, &HotelAPI::updateRoomsRequired, &on_updateRoomsRequired);
    QObject::connect(api, &HotelAPI::queryRoomsFinished, &on_queryRoomsFinished);

    api->queryRooms();

    return a.exec();
}
