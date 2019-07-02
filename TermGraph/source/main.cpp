#include "mainwindow.h"
#include <QApplication>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QQuickStyle::setStyle("Material");

    MainWindow w;

    return a.exec();
}
