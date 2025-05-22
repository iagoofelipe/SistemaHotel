#include "controller.h"

#include <QStatusBar>

Controller::Controller(Model *model, MainWindow *window, QObject *parent)
    : QObject{parent}
    , model(model)
    , window(window)
{
    connect(model, &Model::initializationFinished, this, &Controller::on_model_initializationFinished);
}

void Controller::initialize() {
    window->initialize();
    model->initialize();
}

void Controller::on_model_initializationFinished(bool success) {
    auto statusBar = window->statusBar();
    statusBar->showMessage(success? "api inicializada com êxito" : "api inicializada com falha");
}
