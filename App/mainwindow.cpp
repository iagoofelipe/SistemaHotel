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
    ui->tableWidQuartos->clear();
    ui->tableWidQuartos->setHorizontalHeaderLabels(HotelAPI::RoomTableLabels);

    show();
}

void MainWindow::setRoomsData(MapRooms* rooms) {

}
