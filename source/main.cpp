#include "mainwindow.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QQuickStyle::setStyle("Material");

    MainWindow w;

    return a.exec();
}
