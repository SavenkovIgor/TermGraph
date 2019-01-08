#include "mainwindow.h"

MainWindow::MainWindow(QObject *parent): QObject(parent)
{
    initElemSizes();
    AppConfig::StdFolderPaths::createDefaultFoldersIfNeed();

    engn = new QQmlApplicationEngine();

    QQuickStyle::setStyle("Material");

    // Database init
    Glb::db = new DBAbstract(AppConfig::StdFolderPaths::defaultDatabaseFilePath());
//    Glb::db->makeStartBaseCheck();

    tagProcessor = new TagProcessor();

    network = new NetworkManager();
    nodesMgr = new NodesManager();
    groupsMgr = new GroupsManager(nodesMgr, network);
    paintManager = new PaintManager();

    scene = new MainScene(groupsMgr, nodesMgr, paintManager);

    // remind = new Reminder(scene->getAllNodes());

    analyze = new WordFreqAnalyze();

    qmlRegisterType<PaintedTerm>();

    engn->rootContext()->setContextProperty("mainObj", this);
    engn->rootContext()->setContextProperty("sceneObj", scene);
    engn->rootContext()->setContextProperty("paintManager", paintManager);
    engn->rootContext()->setContextProperty("networkManager", network);
    engn->rootContext()->setContextProperty("groupsManager", groupsMgr);
    engn->rootContext()->setContextProperty("nodesManager", nodesMgr);
    engn->rootContext()->setContextProperty("tagProcessor", tagProcessor);
    engn->load(QUrl("qrc:/qml/MainWindow.qml"));
}

MainWindow::~MainWindow()
{
    delete analyze;
    delete scene;
    delete paintManager;
    delete groupsMgr;
    delete nodesMgr;
    delete network;
    delete tagProcessor;
    delete Glb::db;
    delete engn;
}

//void MainWindow::saveAppSettings()
//{
//    QSettings sett(AppConfig::StdFolderPaths::defaultConfigFilePath(), QSettings::IniFormat);
//}

//void MainWindow::loadAppSettings()
//{
//    QSettings sett(AppConfig::StdFolderPaths::defaultConfigFilePath(), QSettings::IniFormat);
//}

int MainWindow::getUiElementSize(QString elementTypeName)
{
    if (elementSizes.contains(elementTypeName))
        // чтобы эти значения не вставлялись. на всякий случай
        return elementSizes[elementTypeName];
    qDebug() << "Отсутствует размер для элемента:" << elementTypeName;
    return 0;
}

void MainWindow::initElemSizes()
{
#if defined( Q_OS_WIN ) || defined( Q_OS_LINUX ) || defined( Q_OS_MACOS )
    // Screen.pixelDencity - pixel in millimeters
    elementSizes["roundButton"] = 12;
    elementSizes["text"] = 5;
    elementSizes["capitalText"] = 4;
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
    elementSizes["text"] = 3;
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
