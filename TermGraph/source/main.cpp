#include "mainwindow.h"
#include <QApplication>
#include <QQuickStyle>

#include "source/Model/TerminGroup/groupgadgetwrapper.h"
#include "source/Model/Termin/nodegadgetwrapper.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QQuickStyle::setStyle("Material");

    qmlRegisterType<GroupGadgetWrapper>();
    qmlRegisterType<NodeGadgetWrapper>();

    MainWindow w;

    return a.exec();
}
