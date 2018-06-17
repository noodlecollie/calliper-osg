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
        main.cpp

HEADERS +=

FORMS +=

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
