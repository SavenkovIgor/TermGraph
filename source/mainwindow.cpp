#include "mainwindow.h"

MainWindow::MainWindow(QObject *parent) :
    QObject(parent)
{
    initElemSizes();
    engn = new QQmlApplicationEngine();

    Glb::cloud = new CloudServicesWrapper();

    QQuickStyle::setStyle("Material");

    loadDbPath();

    db = new DBAbstract(dbPath);

    TermNode::db = db;
    TermInfo::db = db;
    TermGroup::db = db;
    MainScene::db = db;

    db->makeStartBaseCheck();

    scene = new MainScene();

    connect(scene,SIGNAL(updateGroupLists()),SLOT(updateGroupLists()));

    updateGroupLists();

    scView = new MyView();

    scView->setScene(scene);
//    Glb::scRef = scene;
    scView->setMouseTracking(true);
    scView->setRenderHint(QPainter::Antialiasing,true);

    remind = new Reminder(scene->getAllNodes());

    analyze = new WordFreqAnalyze();

    connect(this,SIGNAL(currGroupNumChanged(int)),SLOT(updGroupAddNode(int)));

    connect(this,SIGNAL(currGroupNumChanged(int)),SLOT(viewGrp(int)));
    connect(engn,SIGNAL(objectCreated(QObject*,QUrl)),SLOT(onQmlCreated(QObject*,QUrl)));

    engn->addImageProvider("sceneimage",scView);
    engn->rootContext()->setContextProperty("mainObj",this);
    engn->rootContext()->setContextProperty("sceneObj",scene);
    engn->rootContext()->setContextProperty("mainViewObj",scView);
    engn->load(QUrl("qrc:/qml/MainWindow.qml"));

}

QString MainWindow::cubizeString(QString str)
{
    for(int i=0,j=0;i<str.size();i++,j++){
        if(j > 20){
            if(str[i] == ' ') {
                str[i] = '\n';
                j=0;
            }
        }
        if(j > 45) {
            str.insert(i,'\n');
            str.insert(i,'-');
            j=0;
        }
    }
    return str;
}

void MainWindow::ctrlMove(QString str)
{
    int index = currGroupNum();

    if( str == "left" )
        index--;
    else if( str == "right" )
        index++;

    index = qBound( 0, index, groupsList().size() - 1 );
    setCurrGroupNum( index );
}

void MainWindow::takeSceneMouse(qreal x, qreal y, qreal winPosX,qreal winPosY,qreal hght, QString actionInfo, QString buttonInfo)
{
    scene->xWindow = winPosX;       //dirty,dirty hack
    scene->yWindow = winPosY + hght;

    QEvent::Type tp;
    if( actionInfo == "press" )
        tp = QEvent::MouseButtonPress;
    else if( actionInfo == "release" )
        tp = QEvent::MouseButtonRelease;
    else if( actionInfo == "move" )
        tp = QEvent::MouseMove;
    else
        tp = QEvent::MouseMove;

    Qt::MouseButton bt;
    static Qt::MouseButton lastPressedBtn;
    if( buttonInfo == "left" ) {
        bt = Qt::LeftButton;
    } else if( buttonInfo == "right" ) {
        bt = Qt::RightButton;
    } else {
        bt = Qt::NoButton;
    }

    if( tp == QEvent::MouseButtonPress ) {
        //Буферизует нажатую кнопку, чтобы указать потом какая была отпущена
        lastPressedBtn = bt;
    } else if ( tp == QEvent::MouseButtonRelease ) {
        bt = lastPressedBtn;
        lastPressedBtn = Qt::NoButton;
    }

    Qt::MouseButtons flg;
    flg.setFlag(bt);

    QMouseEvent *evt = new QMouseEvent( tp, QPointF(x,y), bt, flg, Qt::NoModifier);

    qApp->sendEvent(scView,evt);

    if( evt != nullptr )
        delete evt;
}


MainWindow::~MainWindow()
{
    saveSett();
}

void MainWindow::saveSett()
{
    QSettings sett("appSettings.ini",QSettings::IniFormat);
    if( currGroupNum() >= 0 && currGroupNum() < getGroupsList().size())
        sett.setValue("currGroup",groupsList()[currGroupNum()]);
    sett.setValue("animationSpeed",TermGroup::getAnimSpeed());
    sett.setValue("dataPath",dbPath);
}

void MainWindow::loadSett()
{
    QSettings sett("appSettings.ini",QSettings::IniFormat);
    QString str = sett.value("currGroup","").toString();
    if( str == "" )
        return;

    setCurrGroupNum( getGroupsList().indexOf(str) );
    //    scene->setAnimSpeed( sett.value("animationSpeed",300).toInt() );
}

void MainWindow::loadDbPath()
{
    QSettings sett("appSettings.ini",QSettings::IniFormat);
    dbPath = sett.value("dataPath","tg.termGraph").toString();
}

QStringList MainWindow::getGroupsList()
{
    return db->groupTbl->getAllGroupsNames();
}

void MainWindow::onQmlCreated(QObject*, QUrl)
{
    loadSett();
}

void MainWindow::updateGroupLists()
{
    QStringList lst = db->groupTbl->getAllGroupsNames();
    setGroupsList( lst );
}

void MainWindow::scaleUp()
{
    scView->scaleUp();
}

void MainWindow::scaleDown()
{
    scView->scaleDown();
}

void MainWindow::addNewPoint()
{
    if( changeNum() != "" )
        setChangeNum("");
}

void MainWindow::viewGrp(int num)
{
    QStringList lst = db->groupTbl->getAllGroupsNames();

    if( num < 0 || num >= lst.size() )
        return;

    scene->showGroup( lst[num] );
}

void MainWindow::sceneChangePoint()
{
    TermNode *nd = scene->getSelected();
    if(nd == nullptr)
        return;

    setChangeNum( QString::number(nd->getUid() ) );
    setCurrGroupNum( groupsList().indexOf(nd->getGroupString()));
}

void MainWindow::updGroupAddNode(int num)
{
    //    if( num < 0 || num >= ui->setGroup->count() )
    //        return;
    setCurrGroupNum( num );
    //    ui->setGroup->setCurrentIndex( num );
}



void MainWindow::openBase()
{
//    QFileDialog *dial = new QFileDialog(this,"Select database",qApp->applicationDirPath(),"Database( *.termGraph *.sqlite )");
//    dial->exec();
//    QStringList lst = dial->selectedFiles();
//    if( lst.isEmpty() )
//        return;

//    db->openDB(lst.first());
//    dbPath = lst.first();
//    scene->updateModel();
//    updateGroupLists();
}

void MainWindow::addNewNode(
        QString name,
        QString forms,
        QString def,
        QString descr,
        QString exam,
        QString groupName)
{
    bool ok = false;
    int change  = changeNum().toInt(&ok);
    if( !ok ) {
        change = db->nodeTbl->addNode(name);
    }

    int groupID = db->groupTbl->getUid( groupName );

    db->nodeTbl->setName(change,name);
    db->nodeTbl->setTForms(change,forms);
    db->nodeTbl->setDefinition (change,def);
    db->nodeTbl->setDescription(change,descr);
    db->nodeTbl->setExamples(change,exam);
    db->nodeTbl->setGroup      (change,groupID);

    scene->updateModel();
    //    ui->statusBar->showMessage("Добавлена вершина с названием " + someName,2500);

    setChangeNum("");
}

void MainWindow::testSlot()
{
    scene->updateModel();
}

void MainWindow::setSceneSize(int width, int height)
{
    scView->retImgSz.setWidth(  width  );
    scView->retImgSz.setHeight( height );
}

void MainWindow::setCurrGroupNum(int currGroupNum)
{
    if (m_currGroupNum == currGroupNum)
        return;

    m_currGroupNum = currGroupNum;
    emit currGroupNumChanged(m_currGroupNum);
}

void MainWindow::setChangeNum(QString changeNum)
{
    if (m_changeNum == changeNum)
        return;

    m_changeNum = changeNum;
    emit changeNumChanged(m_changeNum);
}

void MainWindow::setGroupsList(QStringList groupsList)
{
    if (m_groupsList == groupsList)
        return;

    m_groupsList = groupsList;
    emit groupsListChanged(m_groupsList);
}

QStringList MainWindow::getGroupTypes() {
    return TermGroup::getTypesNames();
}

int MainWindow::getUiElementSize(QString elementTypeName)
{
    if( elementSizes.contains(elementTypeName) )
        //чтобы эти значения не вставлялись. на всякий случай
        return elementSizes[elementTypeName];
    qDebug()<<"Отсутствует размер для элемента:"<<elementTypeName;
    return 0;
}

void MainWindow::initElemSizes()
{
#if defined( Q_OS_WIN ) || defined( Q_OS_LINUX )
    //Screen.pixelDencity - pixel in millimeters
    elementSizes["roundButton"] = 16;
    elementSizes["text"] = 5;
    elementSizes["capitalText"] = 4;
    elementSizes["inputLabel"] = 7;
    elementSizes["inputText"] = 6;
    elementSizes["combo"] = 5;
    elementSizes["button"] = 5;
    elementSizes["colSpace"] = 4;
    elementSizes["appHeader"] = 9;
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
#endif

#if defined( Q_OS_MACOS )
    elementSizes["roundButton"] = 16;
    elementSizes["text"] = 5;
    elementSizes["capitalText"] = 4;
    elementSizes["inputLabel"] = 7;
    elementSizes["inputText"] = 6;
    elementSizes["combo"] = 5;
    elementSizes["button"] = 5;
    elementSizes["colSpace"] = 4;
    elementSizes["appHeader"] = 9;
#endif
}
