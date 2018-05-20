#include "mainscene.h"

DBAbstract* MainScene::db = nullptr;

MainScene::MainScene() : QGraphicsScene()
{
    sceneRhytm.setSingleShot(false);
    sceneRhytm.setInterval(30);
    connect(&sceneRhytm,SIGNAL(timeout()),SLOT(checkGroupGeometry()));

    selectTimer.setSingleShot(false);
    selectTimer.setInterval(200);
    connect(&selectTimer,SIGNAL(timeout()),SLOT(checkSelection()));

//    viewGrpTimer.setSingleShot(false);
//    connect(&viewGrpTimer,SIGNAL(timeout()),SLOT(showGroup()));

    setItemIndexMethod(NoIndex);
    updateModel();
}

MainScene::~MainScene()
{
}

void MainScene::initGroups()
{
    QList<int> uids = db->groupTbl->getAllGroupsUid();
    for( int uid : uids ) {

        QSqlRecord rec = db->groupTbl->getGroup( uid );

        if(rec.count() == 0)
            continue;

        TermGroup *newGroup = new TermGroup( rec );

        addItem(newGroup->baseRect);
        addItem(newGroup->groupRect);
        //        addItem(newGroup->treeRect);
        addItem(newGroup->grNmItem);
        //        addItem(newGroup->centerRect);

        if(newGroup->getType() == GroupType::terms) {
            //            EdgesList eLst = newGroup->searchConnections();
            //            for( Edge *e : eLst ) {
            //                addEdge(e->getRoot(),e->getBrnch(),false);
            //            }
            allEdgesList << newGroup->searchConnections();
        }
        groupList << newGroup;
    }
}

void MainScene::destructGroups()
{
    for( TermGroup *g : groupList ) {

        removeItem(g->baseRect);
        removeItem(g->treeRect);
        removeItem(g->groupRect);
        removeItem(g->grNmItem);
//        delete g;
    }

    groupList.clear();
}

void MainScene::formAllNodeList()
{

    for( TermGroup *g : groupList )
        allNodesList << g->getNodeList();
}

void MainScene::loadTermEdges()
{
    QList< QPair<int,int> > edgesLoad = db->edgeTbl->getAllEdges();

    for( int i = 0; i < edgesLoad.size(); i++ ) {
        int from = edgesLoad[i].first;
        int to   = edgesLoad[i].second;

        TermNode *fromNd = getNodeByUid( from );
        TermNode *toNd   = getNodeByUid( to   );

        if(fromNd == nullptr || toNd == nullptr) {
            qDebug()<<"Has invalid edges"<<from<<to;
            continue;
        }

        allEdgesList << new Edge( fromNd, toNd );
    }

}

void MainScene::appendEdgesToScene()
{
    for( Edge *i : allEdgesList )
        addItem( i );
}

NodesList MainScene::getAllNodes()
{
    return allNodesList;
}

TermGroup *MainScene::getGroup(QString name)
{
    for( TermGroup *g : groupList )
        if( g->getName() == name )
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
        for( TermNode* n: allNodesList ) {
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

    for( Edge *e : allEdgesList ) {
        removeItem(e);
    }

    allEdgesList.clear();

    for( TermNode *n : allNodesList ) {
        removeItem(n);
    }

    allNodesList.clear();

    destructGroups();

    initGroups();
    formAllNodeList();
    loadTermEdges();

    appendEdgesToScene();

    for( TermGroup *g : groupList ) {
        g->setLevels();
        g->setTreeCoords();
        g->setNeighbours();
        g->setOrphCoords();
    }

    selectTimer.start();
//    viewGrpTimer.start(200);
    sceneRhytm.start();
    startAllGroupTimers();
}

void MainScene::checkGroupGeometry()
{
    QRectF allRect;
    int y = 0;
    for( int i = 0; i < groupList.size(); i++) {
        groupList[i]->updGroupFrame();

        QRectF baseR = groupList[i]->baseRect->rect().translated(groupList[i]->baseRect->scenePos());
        QRectF grpR = groupList[i]->groupRect->rect().translated(groupList[i]->groupRect->scenePos());

        y += qAbs(baseR.topLeft().y() - grpR.topLeft().y());

        groupList[i]->setBasePoint(QPointF(0,y));

        y += qAbs(baseR.topLeft().y() - grpR.bottomRight().y()) + 40;
        if( allRect.isNull() )
            allRect = groupList[i]->groupRect->rect();
        allRect = allRect.united(groupList[i]->groupRect->rect());
    }

    int mV = 25;
    QMarginsF mrg(mV,mV,mV,mV);
    setSceneRect(allRect.marginsAdded(mrg));

}

void MainScene::dropSelectedNode()
{
    TermNode *nd = getSelected();
    if( nd == nullptr )
        return;

    db->edgeTbl->deleteAllEdgesFor(nd->getUid(),-1);
    db->edgeTbl->deleteAllEdgesFor(-1,nd->getUid());
    db->nodeTbl->deleteNode( nd->getUid() );
    updateModel();
}

void MainScene::addNewGroup(QString name, QString comment, int type )
{
    if ( !db->groupTbl->addGroup( name, comment, type ) ) {
        showMessage( "Название группы не уникально", 2500 );
        return;
    }
    updateGroupLists();
}

void MainScene::deleteGroup(QString name)
{
    db->groupTbl->deleteGroup( name );
    updateGroupLists();
}

void MainScene::exportGrpToJson(QString grpName)
{
    TermGroup* g = getGroup( grpName);
    if ( g == nullptr )
        return;

    QJsonDocument doc = g->getJsonDoc();
    QClipboard *clp = qApp->clipboard();
    clp->setText(doc.toJson());
}

void MainScene::addNewNode(QString name, QString forms, QString def, QString descr, QString exam, QString groupName)
{
    //TODO: Тоже фигня. Нельзя искать в базе по имени группы!
    if( !db->groupTbl->isGroupExistWithUid(groupName) ) {
        qDebug()<<"Группа не найдена";
        return;
    }

    QString nodeId = db->nodeTbl->addNode(name);
    changeNode(nodeId, name, forms, def, descr, exam, groupName);
}

void MainScene::changeNode(QString nodeUuid,
        QString name,
        QString forms,
        QString def,
        QString descr,
        QString exam,
        QString groupName)
{
    //TODO: Тоже фигня. Нельзя искать в базе по имени группы!
    if( !db->groupTbl->isGroupExistWithUid(groupName) ) {
        qDebug()<<"Группа не найдена";
        return;
    }

    int groupID = db->groupTbl->getUid( groupName );

    db->nodeTbl->setName(nodeUuid, name);
    db->nodeTbl->setWordForms(nodeUuid, forms);
    db->nodeTbl->setDefinition (nodeUuid, def);
    db->nodeTbl->setDescription(nodeUuid, descr);
    db->nodeTbl->setExamples(nodeUuid, exam);
    db->nodeTbl->setGroup(nodeUuid, groupID);

    updateModel();
}

QString MainScene::getGroupString(QString grp)
{
    TermGroup* g = getGroup( grp );
    if ( g != nullptr )
        return g->getTypeString();

    return "";
}

void MainScene::viewGrp(int num)
{
    QStringList lst = db->groupTbl->getAllGroupsNames();

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

TermNode *MainScene::getNodeByUid( int uid )
{
    for( TermNode *n : allNodesList )
        if( n->getUid() == uid )
            return n;

    return nullptr;
}

NodesList MainScene::getAllTermsAtPoint(int x, int y) {
    NodesList ret;
    for( TermNode *n : allNodesList ) {
        if( n->getMainRect().contains( x,y ) ) {
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

    for( TermGroup *g : groupList ) {
        if( g->getName().contains(grp) ) {
            QPointF pt = g->groupRect->rect().center();

            QList< QGraphicsView * > v = views();

            for( QGraphicsView *view : v ) {
                view->ensureVisible(QRectF(pt,QSize(1,1)));
                view->centerOn(pt);
            }
        }
    }
    lastGrp = grp;
}

void MainScene::setAnimSpeed(int val)
{
    for( TermGroup *g : groupList ) {
        g->setAnimSpeed( val );
    }
}

QStringList MainScene::getGroupsNames()
{
    return db->groupTbl->getAllGroupsNames();
}

void MainScene::dropEdgesOnSelected()
{
    TermNode *nd = getSelected();
    if( nd == nullptr )
        return;

    db->edgeTbl->deleteAllEdgesFor(nd->getUid(),-1);
    db->edgeTbl->deleteAllEdgesFor(-1,nd->getUid());
    updateModel();

}

void MainScene::checkSelection()
{
    bool someSel = false;
    for( TermNode *n : allNodesList ) {
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
            for( TermNode *n : allNodesList )
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
    ret<<"Uid:"<<QString::number( nd->getUid() );
    ret<<nd->getDebugString();
//    for( EdgesList *e : nd->ed)

    return ret.join(" ");
}

QString MainScene::getCurrNodeLongUid()
{
    TermNode* nd = getSelected();
    if( nd == nullptr )
        return "";

    return nd->getLongUid();
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

void MainScene::changeOrientation()
{
    if( Glb::ori == Qt::Horizontal )
        Glb::ori = Qt::Vertical;
    else
        Glb::ori = Qt::Horizontal;

    updateModel();
}

void MainScene::createTestGroups()
{
    addNewGroup("TestGroup1","",GroupType::terms);

    addNewNode("1","","","","","TestGroup1");
    addNewNode("2","","","","","TestGroup1");
    addNewNode("3","","{1}{2}","","","TestGroup1");
    addNewNode("4","","{1}{2}","","","TestGroup1");
    addNewNode("5","","{1}","","","TestGroup1");
    addNewNode("6","","{5}","","","TestGroup1");
    addNewNode("7","","{5}","","","TestGroup1");
    addNewNode("8","","{6}{7}","","","TestGroup1");
    addNewNode("9","","","","","TestGroup1");

}
