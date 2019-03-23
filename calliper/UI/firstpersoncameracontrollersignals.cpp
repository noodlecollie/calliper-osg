#include "firstpersoncameracontrollersignals.h"

FirstPersonCameraControllerSignals::FirstPersonCameraControllerSignals(QObject *parent)
    : QObject(parent)
{

}

void FirstPersonCameraControllerSignals::notifyUpdated()
{
    emit updated();
}
