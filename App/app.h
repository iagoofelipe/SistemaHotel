#ifndef APP_H
#define APP_H

#include <QApplication>
#include "model.h"
#include "mainwindow.h"
#include "controller.h"

class App : public QApplication
{
    Q_OBJECT

public:
    App(int argc, char **argv);
    int exec();

private:
    Model *model;
    MainWindow *window;
    Controller *controller;
};

#endif // APP_H
