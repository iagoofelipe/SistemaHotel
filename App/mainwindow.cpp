#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Model *model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(model)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialize() {
    clearQuartos();
    show();
}

void MainWindow::setRoomsData(api::MapRooms* rooms) {}

void MainWindow::setGuestsData(api::MapGuests* guests) {}

void MainWindow::setRoomsData(api::MapRooms* rooms) {}

void MainWindow::clearRooms() {
    ui->tableWidQuartos->clear();
    ui->tableWidQuartos->setHorizontalHeaderLabels(api::HotelAPI::RoomTableLabels);
    ui->tableWidQuartos->setColumnCount(api::HotelAPI::RoomTableLabels.size());
}

void MainWindow::clearGuests() {
    ui->tableWidGuests->clear();
    ui->tableWidGuests->setHorizontalHeaderLabels(api::HotelAPI::GuestsTableLabels);
    ui->tableWidGuests->setColumnCount(api::HotelAPI::GuestsTableLabels.size());
}

void MainWindow::clearReserves() {
    ui->tableWidGuests->clear();
    ui->tableWidGuests->setHorizontalHeaderLabels(api::HotelAPI::ReservesTableLabels);
    ui->tableWidGuests->setColumnCount(api::HotelAPI::ReservesTableLabels.size());
}
