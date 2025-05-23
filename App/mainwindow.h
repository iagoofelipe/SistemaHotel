#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "model.h"
#include "hotelapi.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Model *model, QWidget *parent = nullptr);
    ~MainWindow();

    void initialize();
    void setRoomsData(api::MapRooms* rooms);
    void setGuestsData(api::MapGuests* guests);
    void setRoomsData(api::MapRooms* rooms);
    
    void clearRooms();
    void clearGuests();
    void clearReserves();

private:
    Ui::MainWindow *ui;
    Model *model;
};
#endif // MAINWINDOW_H
