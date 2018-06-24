#include "orthographiccameracontrollersignals.h"

OrthographicCameraControllerSignals::OrthographicCameraControllerSignals(QObject *parent)
    : QObject(parent)
{
}

void OrthographicCameraControllerSignals::notifyUpdated()
{
    emit updated();
}
