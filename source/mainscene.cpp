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

    lineItm = new QGraphicsLineItem( nullptr );
    lineItm->setVisible( false );

//    viewGrpTimer.setSingleShot(false);
//    connect(&viewGrpTimer,SIGNAL(timeout()),SLOT(showGroup()));

    setItemIndexMethod(NoIndex);
    updateModel();
}

MainScene::~MainScene()
{
    delete lineItm;
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

    if(editEdge){
        lineItm->setLine(QLineF(startEdgePt,evt->scenePos()));
        lineItm->update();
    }

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
    if(editEdge) {
        sceneRhytm.stop();
        stopAllGroupTimers();
        addItem(lineItm);
        startEdgePt = evt->scenePos();
        lineItm->setVisible(true);
        lineItm->setLine(QLineF(evt->scenePos(),evt->scenePos()));
        evt->accept();
    } else {
        startEdgePt = QPointF();
    }

    QGraphicsScene::mousePressEvent(evt);
}

void MainScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *evt)
{
    //    qDebug()<<"release";
    mouseInfo("release");
    evt->setScreenPos(evt->screenPos() - QPoint(xWindow,yWindow));
    if(editEdge) {
        removeItem(lineItm);
        endEdgePt = evt->scenePos();
        lineItm->setVisible(false);
        setSceneRect(sceneRect());
        evt->accept();
    } else {
        endEdgePt = QPointF();
        //            startEdgePt = QPointF();
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
    }

    if(!startEdgePt.isNull() && !endEdgePt.isNull() && startEdgePt != endEdgePt)
        addEdge(startEdgePt,endEdgePt);

    QGraphicsScene::mouseReleaseEvent(evt);
}


void MainScene::setEditEdgeMode(bool set)
{

    editEdge = set;
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

//TODO: Сделать отдельную!!! функцию для модификации и отдельную для добавления
// Дичь какая-то...
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

void MainScene::addEdge(QPointF rtPt, QPointF brPt)
{
    TermNode *root = nullptr;
    TermNode *bran = nullptr;

    //    (TermNode *)itemAt( rtPt,QTransform() );
    //    (TermNode *)itemAt( brPt,QTransform() );
    for( TermNode *n : allNodesList ) {
        if( n->getMainRect().contains( rtPt ) )
            root = n;
        if( n->getMainRect().contains( brPt ) )
            bran = n;
    }

    addEdge( root, bran );
}

void MainScene::addEdge(TermNode *fr, TermNode *to, bool refresh)
{
    if( fr == nullptr || to == nullptr )
        return;

    if( fr->getUid() == to->getUid() )
        return; //одна и та же вершина

    for( Edge *e : allEdgesList ) {
        if( e->isSameEdge( fr, to ) ) {
            showMessage("Такое ребро уже существует",2000);
            return;
        }
    }

    bool rootIsScheme = ( fr->isRoot() || fr->hasConnections() );
    bool brncIsScheme = ( to->isRoot() || to->hasConnections() );

    //    4 types:

    if( rootIsScheme && !brncIsScheme){ //    1)sch ->node
        addNewEdgeToModel(fr,to);

    } else if(!rootIsScheme && brncIsScheme) { //    2)node->sch
        showMessage("Инвертируем связь", 2000);
        addNewEdgeToModel(to,fr);

    } else if(!rootIsScheme && !brncIsScheme) {//    3)node->node
        showMessage("Нельзя создавать связи вне схемы",2000);
        return;

    } else if(rootIsScheme && brncIsScheme) {//    4)sch ->sch
        if(fr->getPaintLevel() > to->getPaintLevel()) {// Нужна инверсия тянут не оттуда
            showMessage("Нужно тянуть от родителя к потомку",2000);
            addNewEdgeToModel(to,fr);
        } else {
            addNewEdgeToModel(fr,to);
        }
    }

    if( refresh )
        updateModel();
}

void MainScene::addNewEdgeToModel(TermNode *from, TermNode *to)
{
    if(from == to)
        return;

    if( from->isRoot() && to->isRoot() ) {
        showMessage("Нельзя создавать связи между корневыми элементами",2000);
        return;
    }

//    allEdgesList << new Edge( from, to );

    db->edgeTbl->addEdge( from->getUid(), to->getUid() );
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
        selectDrop();
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

QString MainScene::getCurrNodeUid()
{
    TermNode* nd = getSelected();
    if( nd == nullptr )
        return "";

    return QString::number( nd->getUid() );
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
    addNewGroup("TestGroup2","",GroupType::terms);
    addNewGroup("TestGroup3","",GroupType::terms);
}
