#include "termnode.h"

bool TermNode::someoneHover  = false;
bool TermNode::someoneSelect = false;

const qreal TermNode::verScale = 0.0200;
const qreal TermNode::vInterv  = 25.0;
const qreal TermNode::hInterv  = 15.0;

DBAbstract *TermNode::db = nullptr;

const QColor TermNode::leafColor = QColor(94,196,99);
const QColor TermNode::rootColor = QColor(255, 153, 0);
const QColor TermNode::orphanColor = QColor(179, 141, 217);
const QColor TermNode::selectedColor = QColor(128,129,176);

QList< Qt::Edge > TermNode::sides;

TermNode::TermNode( QSqlRecord rec ):
    TermInfo( rec ),
    QGraphicsItem()
{
    if( sides.isEmpty() ) {
        sides << Qt::BottomEdge;
        sides << Qt::TopEdge;
        sides << Qt::RightEdge;
        sides << Qt::LeftEdge;
    }

    adjustSizeForName();

    repNum          = rec.value( db->nodeTbl->remindNum).toInt();
    atLearn         = rec.value( db->nodeTbl->atLearn).toString() == "1";
    lastRepeatDate  = QDate::fromString(rec.value( db->nodeTbl->lastRemind).toString() );

//    setFlag( QGraphicsItem::ItemIsSelectable,true );
//    setFlag(QGraphicsItem::ItemIsMovable,false);

    ndMenu = new QMenu();

    toLearnAct = new QAction();
    connect(toLearnAct,SIGNAL(triggered(bool)),SLOT(swithcAtLearnVar()));
    ndMenu->addAction(toLearnAct);

    setAcceptHoverEvents(true);
    setZValue(1);
}

TermNode::~TermNode()
{
    delete toLearnAct;
    delete ndMenu;
}

void TermNode::adjustSizeForName()
{
    prepareGeometryChange();
    mainRect.setWidth( getNameWidth() + 16 );
    mainRect.setHeight( 4 + getNameHeight() );
}

bool TermNode::isLearning()
{
    return atLearn;
}

TermNode *TermNode::getNearestLeftNeigh()
{
    TermNode* ret = nullptr;
    qreal diff = 100000.0;

    QRectF myRect = getMainRect();
    QRectF neighRect;
    for( TermNode *t : neighbourNodes ) {

        neighRect = t->getMainRect();

        if( neighRect.center().x() > myRect.center().x() )
            continue;

        if( qAbs( neighRect.center().x() - myRect.center().x() ) < diff ) {
            diff = qAbs( neighRect.center().x() - myRect.center().x() );
            ret = t;
        }
    }
    return ret;
}

TermNode *TermNode::getNearestRightNeigh()
{
    TermNode* ret = nullptr;
    qreal diff = 100000.0;

    QRectF myRect = getMainRect();
    QRectF neighRect;
    for(TermNode *t:neighbourNodes) {

        neighRect = t->getMainRect();

        if( neighRect.center().x() < myRect.center().x() )
            continue;

        if( qAbs( neighRect.left() - myRect.right() ) < diff ) {
            diff = qAbs( neighRect.left() - myRect.right() );
            ret = t;
        }
    }
    return ret;
}

EdgesList TermNode::getEdgesInLayer()
{
    EdgesList ret;
    for( TermNode *t : neighbourNodes ) {
        ret << t->edgesToRoots;
        ret << t->edgesToLeafs;
    }

    EdgesList ret2;
    for( Edge *e : ret ) {
        if( e->getLayerDistance() == 1 )
            ret2 << e;
    }
    return ret2;
}

QPointF TermNode::getCenter()
{
    return this->scenePos() + mainRect.center();
}

QString TermNode::getGroupString()
{
    return db->groupTbl->getName( getGroupID() );
}

QRectF TermNode::boundingRect() const
{
    return mainRect;
}

QRectF TermNode::getMainRect( bool localPos ) const
{
    QRectF ret = mainRect;
    if( localPos )
        ret = ret.translated(this->pos());
    else
        ret = ret.translated(scenePos());
    return ret;
}

QRectF TermNode::getRcWithBorders()
{
    //    qreal val = mainRect.width()*0.08;
    qreal val = qBound(0.0,mainRect.width()*0.1,8.0);
    QMarginsF mrg(val,val,val,val);
    QRectF ret = mainRect;
    ret = ret.marginsAdded(mrg);
    ret = ret.translated( this->scenePos() );
    return ret;
}

QSizeF TermNode::getSize(bool withBorder)
{
    qreal val;
    if( Glb::isVertical() )
        val = qBound(0.0,mainRect.width()*0.1,8.0);
    else
        val = qBound(0.0,mainRect.height()*0.2,8.0);
    QMarginsF mrg(val,val,val,val);
    QRectF ret = mainRect;
    if(withBorder)
        ret = ret.marginsAdded(mrg);
    return ret.size();
}

QLineF TermNode::getRectLine(Qt::Edge sd)
{
    QRectF rc = getMainRect();

    switch ( sd ) {
    case Qt::TopEdge:
        return QLineF(rc.topLeft(),rc.topRight());

    case Qt::RightEdge:
        return QLineF(rc.topRight(),rc.bottomRight());

    case Qt::BottomEdge:
        return QLineF(rc.bottomLeft(),rc.bottomRight());

    case Qt::LeftEdge:
        return QLineF(rc.topLeft(),rc.bottomLeft());

    }
    return QLineF();
}

NodeType TermNode::getNodeType()
{
    if (edgesToRoots.isEmpty()) {
        if (edgesToLeafs.isEmpty()) {
            return NodeType::orphan; //Оба пустые
        } else {
            return NodeType::root; //Вниз связей нет, вверх - есть
        }
    } else {
        if (edgesToLeafs.isEmpty()) {
            return NodeType::endLeaf; //Вниз есть, а вверх - нету
        } else {
            return NodeType::middleLeaf; //Есть и вверх и вниз
        }
    }
    return NodeType::root;
}

QColor TermNode::getBaseColor()
{
    switch ( getNodeType() ) {
    case NodeType::orphan: return orphanColor;
    case NodeType::root: return rootColor;
    case NodeType::endLeaf: return leafColor;
    case NodeType::middleLeaf: return leafColor;
    default: return leafColor;
    }
}

void TermNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //defaults
    QRectF rcBase = mainRect;
    QColor col = getBaseColor();
    int transpVal = 100;
    setZValue(1);

    if( someoneHover || someoneSelect ) {
        if( thisHovered || isSelected() || relative ) {
            if( isSelected() || thisHovered )
                col = selectedColor;
            col.setAlpha( 255 );
            setZValue(2);
        } else {
            col.setAlpha( transpVal );
            setZValue(1);
        }
    }

    QString str;
    str += getSmallName();

    //    str += ":" + QString::number( uid );
    //    str += " (" + QString::number( paintLevel ) + ")";
    //    str += " (" + QString::number( neighboursList.size() ) + ")";
    //    str += " (" + QString::number( edgesUpList.size() ) + ")";

    //    EdgeRefList ed;
    //    ed<<edgesUpList;
    //    ed<<edgesDownList;
    //    str += " :" + QString::number(ed.count());
    //    str += " :" + QString::number( personalAddInterval );
//    str += " :" + Glb::ptToStr( scenePos() );
    //    QString testStrInner = " :" + QString::number( personalAddInterval );
    //    QString testStrInner = " :" + QString::number( newPosOffs );

    QBrush br( col, Qt::SolidPattern );

    painter->setPen(QColor(0,0,0,col.alpha()));
    painter->setBrush(br);

    if ( thisHovered )
        painter->drawRoundedRect(rcBase.adjusted(-1,-1,1,1),5,5);
    else
        painter->drawRoundedRect(rcBase, 5, 5);

    painter->drawText(rcBase.adjusted(1,0,1,0),Qt::AlignCenter,str);
    
    if( atLearn ) {
        QRectF miniRc = QRectF(rcBase.topLeft(),QSizeF(11,14));
        painter->setBrush(QColor(245,222,179));
        painter->drawRoundedRect(miniRc,1,1);
        painter->drawText(rcBase.topLeft() + QPointF(2,12),QString::number(repNum));
    }
}

void TermNode::setRelatPaint(bool val)
{
    for(TermNode *n:rootNodes)
        n->relative = val;

    for(TermNode *n:leafNodes)
        n->relative = val;

    for(Edge *e:edgesToRoots)
        e->wide = val;

    for(Edge *e:edgesToLeafs)
        e->wide = val;
}

void TermNode::hoverEnterEvent(QGraphicsSceneHoverEvent *evt)
{
    thisHovered = true;
    someoneHover = true;

    relative = true;

    setRelatPaint(true);

    QGraphicsItem::hoverEnterEvent(evt);
}

void TermNode::hoverLeaveEvent(QGraphicsSceneHoverEvent *evt)
{
    thisHovered = false;
    someoneHover = false;

    relative = false;

    if( !isSelected() )
        setRelatPaint(false);

    QGraphicsItem::hoverLeaveEvent(evt);
}

void TermNode::mousePressEvent(QGraphicsSceneMouseEvent *evt)
{
    if (evt->button() == Qt::RightButton){
        if( atLearn )
            toLearnAct->setText("Снять с изучения");
        else
            toLearnAct->setText("Изучить");

        stopGroupAnimation();
        ndMenu->exec(evt->screenPos());
        return;
    }
    QGraphicsItem::mousePressEvent(evt);

}

bool TermNode::hasConnections()
{
    return !( rootNodes.isEmpty() && leafNodes.isEmpty() );
}

bool TermNode::hasConnectionsInGroup()
{
    for( Edge *e : edgesToLeafs ) {
        if( e->getLeaf()->getGroupID() == getGroupID() ) {
            return true;
        }
    }

    for( Edge *e : edgesToRoots ) {
        if( e->getLeaf()->getGroupID() == getGroupID() ) {
            return true;
        }
    }
    return false;
}

void TermNode::setLevel(int lev)
{
    if(lev > paintLevel)
        paintLevel = lev;

    for(TermNode *t:leafNodes) {
        if( getGroupID() != t->getGroupID() )
            continue;
        t->setLevel( lev + 1 );
    }
}

int TermNode::getUpLevels(int pLevel)
{
    Q_UNUSED(pLevel) //TODO: check!
    int ret = -1;
    for( TermNode *n : leafNodes ){
        ret = qMax( ret, n->getUpLevels() );
    }

    if( ret == -1 )
        ret = getPaintLevel();

    return ret;
}

void TermNode::clearNeighbours()
{
    neighbourNodes.clear();
}

void TermNode::clearConnBrRootLists()
{
    rootNodes.clear();
    leafNodes.clear();
}

void TermNode::addToNeighboursList(TermNode *t)
{
    neighbourNodes << t;
}

void TermNode::addEdgeRef(Edge *edge)
{
    if( edge->getRoot() == this && edge->getLeaf() != this) { //We are source - connection up
        edgesToLeafs    << edge;
        leafNodes << edge->getLeaf();
    }

    if( edge->getLeaf() == this && edge->getRoot() != this) { //We are acceptor - connection down
        edgesToRoots << edge;
        rootNodes  << edge->getRoot();
    }
}

int TermNode::getPaintLevel() { return paintLevel; }

void TermNode::countForces()
{
    if(!hasConnections() && !isRoot())
        return;

    qreal edForce = 0.0;
    qreal myX;
    if( Glb::isVertical() )
        myX = getCenter().x();
    else
        myX = getCenter().y();
    qreal tmp = 0.0;
    qreal notMyPos = 0.0;

    EdgesList edges;
    edges << edgesToRoots;
    edges << edgesToLeafs;

    for( Edge *e : edges ) {
        if( !e->isInGroupEdge() )
            continue;

        if( Glb::isVertical() )
            tmp = e->getXProjection();
        else
            tmp = e->getYProjection();

        if( Glb::isVertical() ) {
            if( e->getRoot() == this )
                notMyPos = e->getLeaf()->getCenter().x();
            else if( e->getLeaf() == this )
                notMyPos = e->getRoot()->getCenter().x();
            else
                continue;
        } else {
            if( e->getRoot() == this )
                notMyPos = e->getLeaf()->getCenter().y();
            else if( e->getLeaf() == this )
                notMyPos = e->getRoot()->getCenter().y();
            else
                continue;
        }

        tmp *= verScale;

        if(notMyPos > myX)
            edForce += tmp;
        else
            edForce += (-1)*tmp;
    }

    //    testStr += "edf:" + QString::number(edForce);

    newPosOffs = 0.0;
    newPosOffs += edForce;
}

int TermNode::getIntersections(bool swapped)
{
    EdgesList edges;
    //    edges << edgesUpList;
    edges << edgesToRoots;

    edges << getEdgesInLayer();

//    qDebug()<<"edges.size"<<this->getName()<<edges.size();

    QList< QPointF > interList;

    for( int i = 0; i < edges.size(); i++ ) {
        for( int j = i+1; j < edges.size(); j++ ) {
            QLineF l1 = edges[i]->getLine( swapped ),
                    l2 = edges[j]->getLine( swapped );

            QPointF *interPt = new QPointF() ;
            if( l1.intersect(l2,interPt) == QLineF::BoundedIntersection ) {

                bool nearFound = false;

                qreal dist = 0.01;
                if(isNearPoints(*interPt,l1.p1(),dist))
                    nearFound = true;
                if(isNearPoints(*interPt,l1.p2(),dist))
                    nearFound = true;
                if(isNearPoints(*interPt,l2.p1(),dist))
                    nearFound = true;
                if(isNearPoints(*interPt,l2.p2(),dist))
                    nearFound = true;

                if(!nearFound) {
                    bool interFound = false;
                    for( QPointF inPt : interList ) {
                        if( isNearPoints(inPt,*interPt,dist) ) {
                            interFound = true;
                            break;
                        }
                    }
                    if( !interFound )
                        interList << *interPt;
                }
            }
            delete interPt;
        }
    }
    return interList.size();
}

qreal TermNode::getSumEdgesLength( bool swap = false)
{
    EdgesList edges;
    edges << edgesToLeafs;
    edges << edgesToRoots;
    qreal ret = 0.0;
    for( Edge *e : edges ) {
        if (!e->isInGroupEdge())
            continue;
        ret += qAbs(e->getLine(swap).dx());
    }
    return ret;
}

void TermNode::setSwap(QPointF toPt)
{
    EdgesList lst;
    lst = getConnectedEdges();

    for( Edge *e : edgesToRoots ) {
        e->swPtBran = toPt;
    }

    for( Edge *e : edgesToLeafs ) {
        e->swPtRoot = toPt;
    }
}

void TermNode::dropSwap()
{
    EdgesList lst;
    lst = getConnectedEdges();

    for( Edge *e : lst ) {
        e->swPtBran = QPointF();
        e->swPtRoot = QPointF();
    }
}

bool TermNode::isNearPoints(QPointF pt1,QPointF pt2,qreal dist) {
    pt1 -= pt2;
    //    pt1.setX(qAbs(pt1.x()));
    //    pt1.setY(qAbs(pt1.y()));
    //    if(qAbs(pt1.x()) <= dist && qAbs(pt1.y()) <= dist)
    //        return true;
    if( pt1.manhattanLength() <= dist )
        return true;
    return false;
}

int TermNode::getRepNum() const
{
    return repNum;
}

bool TermNode::isRoot() {
    return getNodeType() == NodeType::root;
}

bool TermNode::isOrphan()
{
    return getNodeType() == NodeType::orphan;
}

bool TermNode::isLeaf()
{
    NodeType type = getNodeType();
    return type == NodeType::endLeaf || type == NodeType::middleLeaf;
}

bool TermNode::isInTree()
{
    return !isOrphan();
}

bool TermNode::needRemindToday()
{
    if( lastRepeatDate.addDays(getNextRepeatOffset(repNum)) <= QDate::currentDate() )
        return true;
    return false;
}

bool TermNode::isRemindDateMissed()
{
    if( lastRepeatDate.addDays(getNextRepeatOffset(repNum)) < QDate::currentDate() )
        return true;
    return false;
}

void TermNode::setRemind(TermNode::KnowLevel lvl)
{
    switch (lvl) {
    case TermNode::dontKnowLvl:
        repNum = qBound(0,repNum-1,10);
        break;

    case TermNode::remindLvl:
        repNum = qBound(0,repNum+1,10);
        break;

    case TermNode::wellRemindLvl:
        repNum = qBound(0,repNum+2,10);
        break;

    }

    lastRepeatDate = QDate::currentDate();
    db->nodeTbl->setRemindNum(getUid(),repNum,QDate::currentDate());

}

bool TermNode::applyMove()
{
    //Если сильно смещение - кидаем true

    TermNode *left = getNearestLeftNeigh();
    TermNode *right = getNearestRightNeigh();

    QRectF myRc = getRcWithBorders();
    myRc = myRc.translated(newPosOffs,0.0);
    QRectF leftRc;
    QRectF rightRc;

    qreal k = 0.01;
    newPosOffs = qBound( -myRc.width()*k, newPosOffs, myRc.width()*k );

    if( newPosOffs < 0 ) {
        if( left != nullptr ) {
            leftRc = left->getRcWithBorders();
            newPosOffs = qBound( leftRc.right() - myRc.left(), newPosOffs, 0.0 );
        }
    }

    if( newPosOffs > 0 ) {
        if( right != nullptr) {
            rightRc = right->getRcWithBorders();
            newPosOffs = qBound( 0.0, newPosOffs, rightRc.left() - myRc.right() );
        }
    }


    if( qAbs(newPosOffs) > 0.05 ) {
        if( Glb::ori == Qt::Vertical )
            moveBy(newPosOffs,0.0);
        else
            moveBy(0.0,newPosOffs);
        return true;
    }

    return false;
}

QRectF TermNode::getTreeSize(QRectF rect)
{
    QPointF p = scenePos();
    QRectF rc = mainRect.translated(p);
    rc = rc.united(rect);
    for( int i = 0; i < leafNodes.size(); i++ ) {
        if( leafNodes[i]->getGroupID() != getGroupID() )
            continue;

        rc = leafNodes[i]->getTreeSize(rc);
    }

    return rc;
}

EdgesList TermNode::getConnectedEdges()
{
    EdgesList ret;
    ret << edgesToLeafs;
    ret << edgesToRoots;
    return ret;
}

QString TermNode::getDebugString() {

    QStringList p;
    QString tmp;

    p << Glb::ptToStr( pos() );

    tmp = " childs:";
    QList<QGraphicsItem*> childs = childItems();
    for( int i = 0; i < childs.size(); i++ ) {
        TermNode *n = (TermNode*)childs[i];
        tmp += " " + QString::number( n->getUid() );
    }
    p << Glb::ptToStr( scenePos() );

    QString addInterf;
    addInterf += QString::number(getIntersections()) + " ";
    addInterf += QString::number(getIntersections(true));
    p << addInterf;

    p << " upLevels:" + QString::number(getUpLevels() - getPaintLevel());
    //        addInterf += QString::number(personalAddInterval) + " ";
    p << getLongUid(true);
    return p.join("\n");
}

void TermNode::swithcAtLearnVar()
{
    atLearn = !atLearn;
    db->nodeTbl->setAtLearn(getLongUid(),atLearn);
}

int TermNode::getNextRepeatOffset(int lvl)
{
    return getLevelDaysFromBase( lvl + 1 ) - getLevelDaysFromBase( lvl );
}

int TermNode::getLevelDaysFromBase(int lvl)
{
    if( lvl <= 0 )
        return 0; //Варианты 0 и 1

    if( lvl >= 10 )
        return 512; //2^9

    lvl--;
    int ret = 1;
    for(int i=0;i<lvl;i++) {
        ret *= 2;
    }
    return ret;
}

