#ifndef FIRSTPERSONCAMERACONTROLLERSIGNALS_H
#define FIRSTPERSONCAMERACONTROLLERSIGNALS_H

#include <QObject>

class FirstPersonCameraControllerSignals : public QObject
{
    friend class FirstPersonCameraController;
    Q_OBJECT
public:
    explicit FirstPersonCameraControllerSignals(QObject *parent = nullptr);

signals:
    void updated();

private:
    void notifyUpdated();
};

#endif // FIRSTPERSONCAMERACONTROLLERSIGNALS_H
