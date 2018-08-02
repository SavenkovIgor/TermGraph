#include "mainwindow.h"

MainWindow::MainWindow(QObject *parent): QObject(parent)
{
    initElemSizes();
    engn = new QQmlApplicationEngine();

    Glb::cloud = new CloudServicesWrapper();

    QQuickStyle::setStyle("Material");

    loadDbPath();

    db = new DBAbstract(dbPath);

    Glb::db = db;

    db->makeStartBaseCheck();

    tagProcessor = new TagProcessor();

    network = new NetworkManager();
    nodesMgr = new NodesManager();
    groupsMgr = new GroupsManager(nodesMgr, network);

    scene = new MainScene(groupsMgr, nodesMgr);

    scView = new MyView();

    scView->setScene(scene);
//    Glb::scRef = scene;
    scView->setMouseTracking(true);
    scView->setRenderHint(QPainter::Antialiasing, true);

    remind = new Reminder(scene->getAllNodes());

    analyze = new WordFreqAnalyze();

    connect(
                engn,
                SIGNAL(objectCreated(QObject*, QUrl)),
                SLOT(onQmlCreated(QObject*, QUrl)));


    engn->addImageProvider("sceneimage", scView);
    engn->rootContext()->setContextProperty("mainObj", this);
    engn->rootContext()->setContextProperty("sceneObj", scene);
    engn->rootContext()->setContextProperty("networkManager", network);
    engn->rootContext()->setContextProperty("groupsManager", groupsMgr);
    engn->rootContext()->setContextProperty("nodesManager", nodesMgr);
    engn->rootContext()->setContextProperty("mainViewObj", scView);
    engn->rootContext()->setContextProperty("tagProcessor", tagProcessor);
    engn->load(QUrl("qrc:/qml/MainWindow.qml"));
}

void MainWindow::takeSceneMouse(
        qreal x,
        qreal y,
        qreal winPosX,
        qreal winPosY,
        qreal hght,
        QString actionInfo,
        QString buttonInfo)
{
    scene->xWindow = winPosX;  // dirty,dirty hack
    scene->yWindow = winPosY + hght;

    QEvent::Type tp;
    if (actionInfo == "press")
        tp = QEvent::MouseButtonPress;
    else if (actionInfo == "release")
        tp = QEvent::MouseButtonRelease;
    else if (actionInfo == "move")
        tp = QEvent::MouseMove;
    else
        tp = QEvent::MouseMove;

    Qt::MouseButton bt;
    static Qt::MouseButton lastPressedBtn;
    if (buttonInfo == "left") {
        bt = Qt::LeftButton;
    } else if (buttonInfo == "right") {
        bt = Qt::RightButton;
    } else {
        bt = Qt::NoButton;
    }

    if (tp == QEvent::MouseButtonPress) {
        //Буферизует нажатую кнопку, чтобы указать потом какая была отпущена
        lastPressedBtn = bt;
    } else if (tp == QEvent::MouseButtonRelease) {
        bt = lastPressedBtn;
        lastPressedBtn = Qt::NoButton;
    }

    Qt::MouseButtons flg;
    flg.setFlag(bt);

    QMouseEvent *evt =
            new QMouseEvent(tp, QPointF(x, y), bt, flg, Qt::NoModifier);

    qApp->sendEvent(scView, evt);

    if (evt != nullptr)
        delete evt;
}

MainWindow::~MainWindow()
{
    saveSett();
}

void MainWindow::saveSett()
{
    QSettings sett("appSettings.ini", QSettings::IniFormat);
    // TODO: заменить индекс группы на uuid
    sett.setValue("animationSpeed", TermGroup::getAnimSpeed());
    sett.setValue("dataPath", dbPath);
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

void MainWindow::loadDbPath()
{
    QSettings sett("appSettings.ini", QSettings::IniFormat);
    dbPath = sett.value("dataPath", "tg.termGraph").toString();
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
    elementSizes["roundButton"] = 16;
    elementSizes["text"] = 5;
    elementSizes["capitalText"] = 4;
    elementSizes["inputLabel"] = 7;
    elementSizes["inputText"] = 6;
    elementSizes["combo"] = 5;
    elementSizes["button"] = 5;
    elementSizes["colSpace"] = 4;
    elementSizes["appHeader"] = 9;
    elementSizes["infoLabel"] = 6;
#endif

#if defined( Q_OS_ANDROID )
    elementSizes["roundButton"] = 8;
    elementSizes["text"] = 3;
    elementSizes["capitalText"] = 4;
    elementSizes["inputLabel"] = 4;
    elementSizes["inputText"] = 4;
    elementSizes["combo"] = 4;
    elementSizes["button"] = 4;
    elementSizes["colSpace"] = 2;
    elementSizes["appHeader"] = 6;
    elementSizes["infoLabel"] = 4;
#endif
}
