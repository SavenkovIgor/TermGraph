#include "mainwindow.h"

MainWindow::MainWindow(QObject *parent): QObject(parent)
{
    initElemSizes();
    AppConfig::StdFolderPaths::createDefaultFoldersIfNeed();

    engn = new QQmlApplicationEngine();

    QQuickStyle::setStyle("Material");

    db = new DBAbstract(AppConfig::StdFolderPaths::defaultDatabaseFilePath());

    Glb::db = db;

    db->makeStartBaseCheck();

    tagProcessor = new TagProcessor();

    network = new NetworkManager();
    nodesMgr = new NodesManager();
    groupsMgr = new GroupsManager(nodesMgr, network);

    scene = new MainScene(groupsMgr, nodesMgr);

//    Glb::scRef = scene;

    // remind = new Reminder(scene->getAllNodes());

    analyze = new WordFreqAnalyze();

    qmlRegisterType<GraphicItemTerm>();

    connect(
                engn,
                SIGNAL(objectCreated(QObject*, QUrl)),
                SLOT(onQmlCreated(QObject*, QUrl)));


    engn->rootContext()->setContextProperty("mainObj", this);
    engn->rootContext()->setContextProperty("sceneObj", scene);
    engn->rootContext()->setContextProperty("paintManager", scene->getPaintManager());
    engn->rootContext()->setContextProperty("networkManager", network);
    engn->rootContext()->setContextProperty("groupsManager", groupsMgr);
    engn->rootContext()->setContextProperty("nodesManager", nodesMgr);
    engn->rootContext()->setContextProperty("tagProcessor", tagProcessor);
    engn->load(QUrl("qrc:/qml/MainWindow.qml"));
}

MainWindow::~MainWindow()
{
    saveSett();
    delete engn;
}

void MainWindow::saveSett()
{
    QSettings sett("appSettings.ini", QSettings::IniFormat);
    // TODO: заменить индекс группы на uuid
//    sett.setValue("animationSpeed", TermGroup::getAnimSpeed());
}

void MainWindow::loadSett()
{
    QSettings sett("appSettings.ini", QSettings::IniFormat);
    QString str = sett.value("currGroup", "").toString();
    if (str == "")
        return;

    // TODO: Переделать заргузку на сцену и на uuid
//    scene->setAnimSpeed( sett.value("animationSpeed",300).toInt() );
}

void MainWindow::onQmlCreated(QObject*, QUrl)
{
    loadSett();
}

void MainWindow::openBase()
{
//    QFileDialog *dial = new QFileDialog(
//                this,
//                "Select database",
//                qApp->applicationDirPath(),
//                "Database( *.termGraph *.sqlite )");
//    dial->exec();
//    QStringList lst = dial->selectedFiles();
//    if( lst.isEmpty() )
//        return;

//    db->openDB(lst.first());
//    dbPath = lst.first();
//    scene->updateModel();
}

void MainWindow::testSlot()
{
    scene->updateModel();
}

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
