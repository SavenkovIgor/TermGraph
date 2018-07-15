#include "mainscene.h"

MainScene::MainScene(GroupsManager* groupsMgr, NodesManager* nodesMgr) : QGraphicsScene()
{
    sceneRhytm.setSingleShot(false);
    sceneRhytm.setInterval(30);

    selectTimer.setSingleShot(false);
    selectTimer.setInterval(200);
    connect(&selectTimer,SIGNAL(timeout()),SLOT(checkSelection()));

//    viewGrpTimer.setSingleShot(false);
//    connect(&viewGrpTimer,SIGNAL(timeout()),SLOT(showGroup()));

    if(groupsMgr == nullptr)
        qDebug() << "Critical error: groupsManager is null!";

    if(nodesMgr == nullptr)
        qDebug() << "Critical error: nodesManager is null!";

    network = new NetworkManager();
    connect(network, SIGNAL(newSyncGroup(QString)), groupsMgr, SLOT(importGroupFromJson(QString)));

    this->groupsMgr = groupsMgr;
    connect(groupsMgr, SIGNAL(groupsListChanged()), SLOT(updateModel()));

    this->nodesMgr = nodesMgr;
    connect(nodesMgr, SIGNAL(nodeChanged()), SLOT(updateModel()));

    setItemIndexMethod(NoIndex);
    updateModel();
}

MainScene::~MainScene()
{
}

void MainScene::initAllGroups()
{
    for( QUuid uuid: groupsMgr->getAllGroupsUuids() ) {
        QSqlRecord rec = groupsMgr->getGroupSqlRecord( uuid );
        if(rec.count() == 0)
            continue;
        addGroupToScene(new TermGroup(rec));
    }
}

void MainScene::addGroupToScene(TermGroup *group)
{
    addItem(group->baseRect);
    connect(&sceneRhytm,SIGNAL(timeout()),group,SLOT(sceneUpdateSignal()));
    groupList << group;
}

void MainScene::deleteAllGroups()
{    
    for( TermGroup* g: groupList )
        removeItem(g->baseRect);

    groupList.clear();
}

NodesList MainScene::getAllNodes()
{
    NodesList ret;
    for( TermGroup* g: groupList )
        ret << g->getAllNodes();
    return ret;
}

TermGroup *MainScene::getGroupByName(QString name)
{
    for( TermGroup* g: groupList )
        if( g->getName() == name )
            return g;

    return nullptr;
}

TermGroup *MainScene::getGroupByUuid(QUuid uuid)
{
    for( TermGroup* g: groupList )
        if( g->getUid() == uuid.toString() )
            return g;

    return nullptr;
}

void MainScene::mouseMoveEvent(QGraphicsSceneMouseEvent *evt)
{
    mouseInfo("move");
    evt->setScreenPos(evt->screenPos() - QPoint(xWindow,yWindow));

    QGraphicsScene::mouseMoveEvent(evt);
}

void MainScene::mousePressEvent(QGraphicsSceneMouseEvent *evt)
{
    //    qDebug()<<"press";
    //setScenePos влияет на позицию курсора в сцене но только во время
    //соотв. события (нажатия клавиши и пр). довольно тупо пытаться
    //это так использовать
    //TODO: Переписать! вынести xWindow в вид по возможности
    evt->setScreenPos(evt->screenPos() - QPoint(xWindow,yWindow));
    //        evt->setScenePos(evt->scenePos() + QPoint(15,yWindow));

    //        mouseInfo( "press at " + Glb::ptToStr(evt->scenePos()) );
    lastPressPt = evt->scenePos();

    QGraphicsScene::mousePressEvent(evt);
}

void MainScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *evt)
{
    //    qDebug()<<"release";
    mouseInfo("release");
    evt->setScreenPos(evt->screenPos() - QPoint(xWindow,yWindow));
    NodesList ndLst = getAllTermsAtPoint(evt->scenePos().x(),evt->scenePos().y());
    if( ndLst.size() == 1 ) {
        int dist = 0;
        dist += qAbs( lastPressPt.x() - evt->scenePos().x() );
        dist += qAbs( lastPressPt.y() - evt->scenePos().y() );

        if( dist <= 30 ) {
            qDebug()<<dist<<lastPressPt<<evt->scenePos();
            ndLst.first()->setFlag( QGraphicsItem::ItemIsSelectable,true );
            ndLst.first()->setSelected(true);
            evt->accept();
        }
    } else {
        for( TermNode* n: getAllNodes() ) {
            n->setFlag( QGraphicsItem::ItemIsSelectable, false );
            n->setSelected( false );
            //                    evt->accept();
        }
    }

    QGraphicsScene::mouseReleaseEvent(evt);
}

void MainScene::updateModel()
{
    selectTimer.stop();
//    viewGrpTimer.stop();
    sceneRhytm.stop();
    stopAllGroupTimers();

    deleteAllGroups();
    initAllGroups();

    for( TermGroup* g: groupList )
        g->sceneUpdateSignal();

    locateGroupsVertically();

    selectTimer.start();
//    viewGrpTimer.start(200);
    sceneRhytm.start();
    startAllGroupTimers();
}

void MainScene::locateGroupsVertically()
{
    int y = 0;

    // Выставляем позиции групп
    for(TermGroup* group: groupList) {

        QRectF baseRc = group->baseRect->rect().translated(group->baseRect->scenePos());
        group->setBasePoint(QPointF(0,y));

        y += baseRc.height() + 40;
    }

    updateSceneRect();
}

void MainScene::updateSceneRect()
{
    QRectF allRect;

    for(TermGroup* group: groupList) {
        if( !group->baseRect->isVisible() )
            continue;

        QRectF baseRc = group->baseRect->rect().translated(group->baseRect->scenePos());
        allRect = allRect.united(baseRc);
    }

    int mV = 50;
    QMarginsF mrg(mV,mV,mV,mV);
    setSceneRect(allRect.marginsAdded(mrg));
}

void MainScene::centerViewOn(QPointF pt)
{
    QList< QGraphicsView * > v = views();

    for( QGraphicsView* view: v ) {
        view->centerOn(pt);
    }
}

void MainScene::deleteSelectedNode()
{
    TermNode *nd = getSelected();
    if( nd == nullptr )
        return;

    nodesMgr->deleteNode(nd->getUuid());
}

void MainScene::exportGrpToJson(QString grpName)
{
    TermGroup* g = getGroupByName( grpName);
    if ( g == nullptr )
        return;

    saveGroupInFolder(g);

    QJsonDocument doc = g->getJsonDoc();
    QClipboard *clp = qApp->clipboard();
    clp->setText(doc.toJson());
}

void MainScene::saveGroupInFolder(TermGroup* g)
{
    if (g == nullptr)
        return;

    QString fileName = g->getName() + " " + g->getUid() + ".grp";
    Glb::saveFile(StdFolderPaths::groupsJsonPath(), fileName, g->getJsonDoc().toJson());
}

QString MainScene::getExportPath()
{
    return StdFolderPaths::groupsJsonPath();
}

QString MainScene::getGroupString(QString grp)
{
    TermGroup* g = getGroupByName( grp );
    if ( g != nullptr )
        return g->getTypeString();

    return "";
}

void MainScene::sendGroupByNetwork(QString groupName)
{
    TermGroup* g = getGroupByName(groupName);
    if ( g == nullptr )
        return;

    network->connectAndSendGroup(receiverIp, g->getJsonDoc());
}

void MainScene::setReceiverHost(QString ip)
{
    receiverIp = ip;
}

void MainScene::showGroup(int num)
{
    QStringList lst = groupsMgr->getAllGroupsNames();

    if( num < 0 || num >= lst.size() )
        return;

    showGroup( lst[num] );
}

TermNode *MainScene::getSelected()
{
    TermNode* ret = nullptr;

    QList<QGraphicsItem *> sel = selectedItems();
    if(sel.size() != 1)
        return ret;

    ret = (TermNode *)sel.first();
    return ret;
}

NodesList MainScene::getAllTermsAtPoint(int x, int y) {
    NodesList ret;
    for( TermNode *n: getAllNodes() ) {
        if( n->getNodeRect(CoordType::scene).contains( x,y ) ) {
            ret << n;
        }
    }
    return ret;
}

void MainScene::showGroup(QString grp)
{
//    if( viewGrpTimer.isActive() ) {
//        timerCount++;
//        if( timerCount > 5 ) {
//            viewGrpTimer.stop();
//            timerCount = 0;
//        }
//    }

    static QString lastGrp;
    if( grp == "" )
        grp = lastGrp;

    if (grp == "Все группы") {
        for( TermGroup *group : groupList )
            group->baseRect->show();

    } else {

        for( TermGroup *group : groupList ) {
            if( group->getName().contains(grp) ) {
                group->baseRect->show();
                QRectF baseRc = group->baseRect->rect().translated(group->baseRect->scenePos());
                centerViewOn(baseRc.center());
            } else {
                group->baseRect->hide();
            }
        }
    }

    updateSceneRect();

    lastGrp = grp;
}

void MainScene::setAnimSpeed(int val)
{
    for( TermGroup *g : groupList ) {
        g->setAnimSpeed( val );
    }
}

void MainScene::checkSelection()
{
    bool someSel = false;
    for( TermNode *n : getAllNodes() ) {
        if(n->isSelected()) {
            someSelected();
            someSel = true;
            TermNode::someoneSelect = true;
            n->setRelatPaint(true);
        }

    }

    if(!someSel) {
        selectionDrop();
        TermNode::someoneSelect = false;
        if(!TermNode::someoneHover)
            for( TermNode *n : getAllNodes() )
                n->setRelatPaint(false);
    }

//    qDebug()<<"count of items"<<this->items().count();
}

void MainScene::startAllGroupTimers()
{
    for( TermGroup *g : groupList )
        g->startAnimation();
}

void MainScene::stopAllGroupTimers()
{
    for( TermGroup *g : groupList )
        g->stopAnimation();
}

QString MainScene::getCurrNodeDebugInfo()
{
    TermNode* nd = getSelected();
    if( nd == nullptr )
        return "";

    QStringList ret;
    ret << "Uuid:" << nd->getUuid().toString();
    ret << nd->getDebugString();
//    for( EdgesList *e : nd->ed)

    return ret.join(" ");
}

QString MainScene::getCurrNodeLongUid()
{
    TermNode* nd = getSelected();
    if( nd == nullptr )
        return "";

    return nd->getUuid().toString();
}

QString MainScene::getCurrNodeName()
{
    TermNode* nd = getSelected();
    if( nd == nullptr )
        return "";

    return nd->getName();
}

QString MainScene::getCurrNodeForms()
{
    TermNode* nd = getSelected();
    if( nd == nullptr )
        return "";

    return nd->getNameFormStr();
}

QString MainScene::getCurrNodeDefinition()
{
    TermNode* nd = getSelected();
    if( nd == nullptr )
        return "";

    return nd->getDefinition();
}

QString MainScene::getCurrNodeDescription()
{
    TermNode* nd = getSelected();
    if( nd == nullptr )
        return "";

    return nd->getDescription();
}

QString MainScene::getCurrNodeExamples()
{
    TermNode* nd = getSelected();
    if( nd == nullptr )
        return "";

    return nd->getExamples();
}

bool MainScene::getCurrNodeIsRoot()
{
    TermNode* nd = getSelected();
    if( nd == nullptr )
        return "";

    return nd->isRoot();
    //TODO: Возможно переделать в функциональном стиле!
}

void MainScene::createTestGroups()
{
    groupsMgr->addNewGroup("TestGroup1","",GroupType::terms);

    nodesMgr->addNewNode("1","","","","","TestGroup1");
    nodesMgr->addNewNode("2","","","","","TestGroup1");
    nodesMgr->addNewNode("3","","{1}{2}","","","TestGroup1");
    nodesMgr->addNewNode("4","","{1}{2}","","","TestGroup1");
    nodesMgr->addNewNode("5","","{1}","","","TestGroup1");
    nodesMgr->addNewNode("6","","{5}","","","TestGroup1");
    nodesMgr->addNewNode("7","","{5}","","","TestGroup1");
    nodesMgr->addNewNode("8","","{6}{7}","","","TestGroup1");
    nodesMgr->addNewNode("9","","","","","TestGroup1");
}
