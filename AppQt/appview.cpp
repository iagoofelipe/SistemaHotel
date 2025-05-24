#include "appview.h"
#include "ui_appview.h"

AppView::AppView(AppModel *model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AppView)
    , model(model)
{
    ui->setupUi(this);

    // gerando conteúdos
    QWidget
        *oldGuests = ui->widContentGuests,
        *oldRooms = ui->widContentRooms,
        *oldReserves = ui->widContentReserves;

    ui->widContentGuests = tableGuests = new Table(ui->tabWidget, "Hóspedes", api::HotelAPI::GuestTableHeaders);
    ui->widContentRooms = tableRooms = new Table(ui->tabWidget, "Quartos", api::HotelAPI::RoomTableHeaders);
    ui->widContentReserves = tableReserves = new Table(ui->tabWidget, "Reservas", api::HotelAPI::ReserveTableHeaders);

    ui->layoutGuests->replaceWidget(oldGuests, tableGuests);
    ui->layoutRooms->replaceWidget(oldRooms, tableRooms);
    ui->layoutReserves->replaceWidget(oldReserves, tableReserves);

    oldGuests->deleteLater();
    oldRooms->deleteLater();
    oldReserves->deleteLater();
}

AppView::~AppView()
{
    delete ui;
}

void AppView::initialize() {
    // tableGuests->clear();
    // tableRooms->clear();
    // tableReserves->clear();
    show();
}

void AppView::showMessage(const QString& msg, int timeout) {
    statusBar()->showMessage(msg, timeout);
}

void AppView::setDataTableReserves(api::MapReserves *reserves) {
    // "Cliente", "Quarto", "Andar", "CheckIn", "CheckOut"
    QList<QList<QVariant>> data;

    for(auto [id, reserve] : reserves->asKeyValueRange()) {
        data.append(QList<QVariant> {
            reserve->guestName,
            reserve->roomName,
            reserve->roomFloor,
            reserve->checkIn.toString("HH:mm dd/MM/yyyy"),
            reserve->checkOut.toString("HH:mm dd/MM/yyyy"),
        });
    }

    tableReserves->setData(data);
}

void AppView::setDataTableGuests(api::MapGuests *guests) {
    // "ID", "Nome", "E-mail"
    QList<QList<QVariant>> data;

    for(auto [id, guest] : guests->asKeyValueRange()) {
        data.append(QList<QVariant> {
            guest->id,
            guest->name,
            guest->email,
        });
    }

    tableGuests->setData(data);
}

void AppView::setDataTableRooms(api::MapRooms *rooms) {
    // "Nome", "Andar", "Valor", "Camas Casal", "Camas Solteiro", "Banheira", "Varanda"
    QList<QList<QVariant>> data;

    for(auto [id, room] : rooms->asKeyValueRange()) {
        data.append(QList<QVariant> {
            room->name,
            room->floor,
            room->nightVal,
            room->numCoupleBed,
            room->numSingleBed,
            room->bathtub? "Sim" : "Não",
            room->balcony? "Sim" : "Não",
        });
    }

    tableRooms->setData(data);
}

Table* AppView::getTableReserves() { return tableReserves; }

Table* AppView::getTableRooms() { return tableRooms; }

Table* AppView::getTableGuests() { return tableGuests; }
