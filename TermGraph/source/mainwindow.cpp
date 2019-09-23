#include "mainwindow.h"

MainWindow::MainWindow(QObject *parent):
    QObject(parent),
    network(new NetworkManager()),
    nodesManager(new NodesManager()),
    paintManager(new PaintManager()),
    analyze(new WordFreqAnalyze()),
    tagProcessor(new TagProcessor()),
    textProcessor(new TextProcessor()),
    groupsManager(new GroupsManager(nodesManager.get())),
    syncManager(new SyncManager(network.get(), groupsManager.get(), nodesManager.get())),
    scene(new MainScene(groupsManager.get(), nodesManager.get(), paintManager.get())),
    qmlEngine(new QQmlApplicationEngine())
{
    initElemSizes();
    AppSettings::StdPaths::createDefaultFoldersIfNeed();

    QQuickStyle::setStyle("Material");

    // Database init
    groupsManager->updateGroupUuidNameMaps();

    // remind = new Reminder(scene->getAllNodes());

    qmlRegisterType<PaintedTerm>();

    qmlEngine->rootContext()->setContextProperty("mainObj", this);
    qmlEngine->rootContext()->setContextProperty("sceneObj", scene.get());
    qmlEngine->rootContext()->setContextProperty("paintManager", paintManager.get());
    qmlEngine->rootContext()->setContextProperty("networkManager", network.get());
    qmlEngine->rootContext()->setContextProperty("groupsManager", groupsManager.get());
    qmlEngine->rootContext()->setContextProperty("nodesManager", nodesManager.get());
    qmlEngine->rootContext()->setContextProperty("tagProcessor", tagProcessor.get());
    qmlEngine->rootContext()->setContextProperty("textProcessor", textProcessor.get());
    qmlEngine->rootContext()->setContextProperty("syncManager", syncManager.get());
    qmlEngine->load(QUrl("qrc:/qml/MainWindow.qml"));
}

int MainWindow::getUiElementSize(const QString &elementTypeName)
{
    if (elementSizes.contains(elementTypeName)) {
        // чтобы эти значения не вставлялись. на всякий случай
        return elementSizes[elementTypeName];
    }
    qDebug() << "Отсутствует размер для элемента:" << elementTypeName;
    return 0;
}

int MainWindow::dbVersion()
{
    return Database::instance().appConfigTable->getDbVersion();
}

void MainWindow::initElemSizes()
{
    if constexpr (AppSettings::Platform::isDesktop()) {
        // Screen.pixelDencity - pixel in millimeters
        elementSizes["roundButton"] = 15;
        elementSizes["text"]        = 5;
        elementSizes["capitalText"] = 6;
        elementSizes["inputLabel"]  = 7;
        elementSizes["inputText"]   = 6;
        elementSizes["combo"]       = 5;
        elementSizes["button"]      = 5;
        elementSizes["colSpace"]    = 4;
        elementSizes["appHeader"]   = 9;
        elementSizes["infoLabel"]   = 6;
        elementSizes["sideMenu"]    = 7;
    }

    if constexpr (AppSettings::Platform::isAndroid()) {
        elementSizes["roundButton"] = 9;
        elementSizes["text"]        = 4;
        elementSizes["capitalText"] = 5;
        elementSizes["inputLabel"]  = 4;
        elementSizes["inputText"]   = 4;
        elementSizes["combo"]       = 4;
        elementSizes["button"]      = 4;
        elementSizes["colSpace"]    = 2;
        elementSizes["appHeader"]   = 6;
        elementSizes["infoLabel"]   = 4;
        elementSizes["sideMenu"]    = 5;
    }
}
