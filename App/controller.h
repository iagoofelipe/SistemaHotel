#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "model.h"
#include "mainwindow.h"

#include <QObject>

class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(Model *model, MainWindow *window, QObject *parent = nullptr);

    void initialize();

signals:


private:
    Model *model;
    MainWindow *window;

    void on_model_initializationFinished(bool success);
};

#endif // CONTROLLER_H
