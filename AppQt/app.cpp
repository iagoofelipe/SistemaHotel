#include "app.h"

App::App(int argc, char **argv)
    : QApplication(argc, argv)
    , model(new AppModel)
    , view(new AppView(model))
    , controller(new AppController(model, view))
{}

int App::exec() {
    controller->initialize();

    return QApplication::exec();
}
