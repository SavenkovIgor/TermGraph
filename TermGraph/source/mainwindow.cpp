#include "mainwindow.h"

MainWindow::MainWindow(QObject *parent):
    QObject(parent),
    network(new NetworkManager()),
    nodesManager(new NodesManager()),
    paintManager(new PaintManager()),
    analyze(new WordFreqAnalyze()),
    tagProcessor(new TagProcessor()),
    textProcessor(new TextProcessor()),
    groupsManager(new GroupsManager(nodesManager.get(), network.get())),
    scene(new MainScene(groupsManager.get(), nodesManager.get(), paintManager.get())),
    qmlEngine(new QQmlApplicationEngine())
{
    initElemSizes();
    AppConfig::StdFolderPaths::createDefaultFoldersIfNeed();

    QQuickStyle::setStyle("Material");

    // Database init
    Glb::db = new DBAbstract(AppConfig::StdFolderPaths::defaultDatabaseFilePath());
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
    qmlEngine->load(QUrl("qrc:/qml/MainWindow.qml"));
}

MainWindow::~MainWindow()
{
    delete Glb::db;
}

//void MainWindow::saveAppSettings()
//{
//    QSettings sett(AppConfig::StdFolderPaths::defaultConfigFilePath(), QSettings::IniFormat);
//}

//void MainWindow::loadAppSettings()
//{
//    QSettings sett(AppConfig::StdFolderPaths::defaultConfigFilePath(), QSettings::IniFormat);
//}

int MainWindow::getUiElementSize(const QString &elementTypeName)
{
    if (elementSizes.contains(elementTypeName)) {
        // чтобы эти значения не вставлялись. на всякий случай
        return elementSizes[elementTypeName];
    }
    qDebug() << "Отсутствует размер для элемента:" << elementTypeName;
    return 0;
}

void MainWindow::initElemSizes()
{
#if defined( Q_OS_WIN ) || defined( Q_OS_LINUX ) || defined( Q_OS_MACOS )
    // Screen.pixelDencity - pixel in millimeters
    elementSizes["roundButton"] = 15;
    elementSizes["text"] = 5;
    elementSizes["capitalText"] = 5;
    elementSizes["inputLabel"] = 7;
    elementSizes["inputText"] = 6;
    elementSizes["combo"] = 5;
    elementSizes["button"] = 5;
    elementSizes["colSpace"] = 4;
    elementSizes["appHeader"] = 9;
    elementSizes["infoLabel"] = 6;
    elementSizes["sideMenu"] = 7;
#endif

#if defined( Q_OS_ANDROID )
    elementSizes["roundButton"] = 9;
    elementSizes["text"] = 4;
    elementSizes["capitalText"] = 4;
    elementSizes["inputLabel"] = 4;
    elementSizes["inputText"] = 4;
    elementSizes["combo"] = 4;
    elementSizes["button"] = 4;
    elementSizes["colSpace"] = 2;
    elementSizes["appHeader"] = 6;
    elementSizes["infoLabel"] = 4;
    elementSizes["sideMenu"] = 5;
#endif
}
