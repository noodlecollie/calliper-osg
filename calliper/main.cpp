#include <QApplication>

#include "UI/mainwindowplaceholder.h"
#include "UI/osgviewwidget.h"

int main(int argc, char** argv)
{
#if QT_VERSION >= QT_VERSION_CHECK(5,6,0)
    QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
#else
    qputenv("QT_DEVICE_PIXEL_RATIO", QByteArray("1"));
#endif

    QApplication qapp(argc, argv);
    MainWindowPlaceholder window;
    window.show();

    return qapp.exec();
}
