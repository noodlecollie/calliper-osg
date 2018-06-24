#ifndef ORTHOGRAPHICCAMERACONTROLLERSIGNALS_H
#define ORTHOGRAPHICCAMERACONTROLLERSIGNALS_H

#include <QObject>

class OrthographicCameraControllerSignals : public QObject
{
    friend class OrthographicCameraController;
    Q_OBJECT
public:
    explicit OrthographicCameraControllerSignals(QObject *parent = nullptr);

signals:
    void updated();

private:
    void notifyUpdated();
};

#endif // ORTHOGRAPHICCAMERACONTROLLERSIGNALS_H
