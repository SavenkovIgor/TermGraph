#include "mainwindow.h"
#include <QApplication>
#include <QQuickStyle>

#ifdef Q_OS_ANDROID
#include <QtSvg>    //Because deployment sometimes just forgets to include this lib otherwise
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QQuickStyle::setStyle("Material");

    MainWindow w;

    return a.exec();
}
