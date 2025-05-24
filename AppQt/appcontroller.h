#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include "appmodel.h"
#include "appview.h"

#include <QObject>

class AppController : public QObject
{
    Q_OBJECT
public:
    explicit AppController(AppModel *model, AppView *view, QObject *parent = nullptr);

    void initialize();

signals:

private:
    AppModel *model;
    AppView *view;

    void on_model_initializationFinished(bool api);
    void on_model_dataReadyGetRooms(const api::Response& response, api::MapRooms *rooms);
    void on_model_dataReadyGetGuests(const api::Response& response, api::MapGuests *guests);
    void on_model_dataReadyGetReserves(const api::Response& response, api::MapReserves *reserves);
    void on_model_requestError(const api::Response& response);
    void on_tableRooms_updateRequired();
    void on_tableGuests_updateRequired();
    void on_tableReserves_updateRequired();
    void on_tableRooms_currentRowChanged(int row);
    void on_tableGuests_currentRowChanged(int row);
    void on_tableReserves_currentRowChanged(int row);
    void on_api_requestRequired(api::EndPoint endpoint);
};

#endif // APPCONTROLLER_H
