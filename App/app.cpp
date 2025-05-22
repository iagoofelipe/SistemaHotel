#include "app.h"

App::App(int argc, char **argv)
    : QApplication{argc, argv}
    , model{new Model}
    , window{new MainWindow{model}}
    , controller{new Controller{model, window}}
{}

int App::exec() {
    controller->initialize();

    return QApplication::exec();
}
