#ifndef APPVIEW_H
#define APPVIEW_H

#include "appmodel.h"
#include "table.h"

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class AppView;
}
QT_END_NAMESPACE

class AppView : public QMainWindow
{
    Q_OBJECT

public:
    AppView(AppModel *model, QWidget *parent = nullptr);
    ~AppView();

    void initialize();
    void showMessage(const QString& msg, int timeout = 0);
    void setDataTableReserves(api::MapReserves *reserves);
    void setDataTableGuests(api::MapGuests *guests);
    void setDataTableRooms(api::MapRooms *rooms);

    Table* getTableReserves();
    Table* getTableRooms();
    Table* getTableGuests();

private:
    Ui::AppView *ui;
    AppModel *model;

    Table *tableReserves;
    Table *tableRooms;
    Table *tableGuests;

signals:
};
#endif // APPVIEW_H
