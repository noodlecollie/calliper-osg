#-------------------------------------------------
#
# Project created by QtCreator 2018-06-17T19:01:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = calliper
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    UI/osgviewwidget.cpp \
    UI/mainviewpanel.cpp \
    UI/mainwindowplaceholder.cpp \
    UI/quadviewportwidget.cpp \
    Model/applicationmodel.cpp \
    UI/viewport3d.cpp \
    UI/viewport2d.cpp \
    UI/orthographiccameracontroller.cpp \
    UI/orthographiccameracontrollersignals.cpp \
    UI/mouseclickdraghandler.cpp \
    Core/globalkeystate.cpp \
    Settings/settingsmanager.cpp \
    Settings/basesettingscategory.cpp \
    Settings/basesetting.cpp \
    Settings/genericsetting.cpp \
    Settings/settingssubcategory.cpp \
    Settings/keybindsetting.cpp \
    UI/uisettings.cpp \
    OSG/osgdefs.cpp

HEADERS += \
    UI/osgviewwidget.h \
    UI/mainviewpanel.h \
    UI/mainwindowplaceholder.h \
    UI/quadviewportwidget.h \
    OSG/osgdefs.h \
    Model/applicationmodel.h \
    Core/globalsingleton.h \
    OSG/pushwarnings.h \
    OSG/popwarnings.h \
    UI/viewport3d.h \
    UI/viewport2d.h \
    UI/orthographiccameracontroller.h \
    UI/orthographiccameracontrollersignals.h \
    UI/mouseclickdraghandler.h \
    Core/globalkeystate.h \
    Settings/settingsmanager.h \
    Settings/basesettingscategory.h \
    Settings/basesetting.h \
    Settings/genericsetting.h \
    Settings/settingssubcategory.h \
    Settings/keybindsetting.h \
    UI/uisettings.h \
    Settings/settingscategoryinstance.h

FORMS += \
    UI/mainviewpanel.ui \
    UI/mainwindowplaceholder.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../submodule-libs/OpenSceneGraph/lib/ -losg
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../submodule-libs/OpenSceneGraph/lib/ -losgd
else:unix: LIBS += -L$$PWD/../submodule-libs/OpenSceneGraph/lib/ -losg

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../submodule-libs/OpenSceneGraph/lib/ -losgViewer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../submodule-libs/OpenSceneGraph/lib/ -losgViewerd
else:unix: LIBS += -L$$PWD/../submodule-libs/OpenSceneGraph/lib/ -losgViewer

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../submodule-libs/OpenSceneGraph/lib/ -losgGA
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../submodule-libs/OpenSceneGraph/lib/ -losgGAd
else:unix: LIBS += -L$$PWD/../submodule-libs/OpenSceneGraph/lib/ -losgGA

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../submodule-libs/OpenSceneGraph/lib/ -lOpenThreads
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../submodule-libs/OpenSceneGraph/lib/ -lOpenThreadsd
else:unix: LIBS += -L$$PWD/../submodule-libs/OpenSceneGraph/lib/ -lOpenThreads

INCLUDEPATH += $$PWD/../submodule-libs/OpenSceneGraph/include
DEPENDPATH += $$PWD/../submodule-libs/OpenSceneGraph/include

INCLUDEPATH += $$PWD/../submodules/OpenSceneGraph/include
DEPENDPATH += $$PWD/../submodules/OpenSceneGraph/include
