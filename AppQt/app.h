#ifndef APP_H
#define APP_H

#include "appmodel.h"
#include "appview.h"
#include "appcontroller.h"

#include <QApplication>

class App : public QApplication
{
    Q_OBJECT
public:
    App(int argc, char **argv);
    int exec();

private:
    AppModel *model;
    AppView *view;
    AppController *controller;
};

#endif // APP_H
