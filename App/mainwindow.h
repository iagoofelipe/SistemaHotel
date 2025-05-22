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
    void setRoomsData(MapRooms* rooms);

private:
    Ui::MainWindow *ui;
    Model *model;
};
#endif // MAINWINDOW_H
