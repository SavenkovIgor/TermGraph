#include "termgroup.h"

DBAbstract *TermGroup::db = nullptr;

int TermGroup::animSpeed = 300;

TermGroup::TermGroup( QSqlRecord rec, QObject *parent) :
    QObject(parent)
{
    QString groupName = rec.value( db->groupTbl->name ).toString();

    this->grUid    = rec.value( db->groupTbl->uid ).toInt();
    this->longUid  = rec.value( db->groupTbl->longUID ).toString();
    this->type     = static_cast<GroupType>( rec.value( db->groupTbl->type ).toInt() );

    baseRect = new QGraphicsRectItem(QRectF(QPoint(0,0),QSizeF(10.0,10.0)),nullptr);
    baseRect->setZValue(1);
    baseRect->setBrush(QColor(0,0,0,60));

    treeRect    = new QGraphicsRectItem( nullptr );
    treeRect->setParentItem(baseRect);
    treeRect->setBrush(QColor(0,255,0,60));

    orphansRect = new QGraphicsRectItem( nullptr );
    orphansRect->setParentItem(baseRect);
    orphansRect->setBrush(QColor(0,0,255,60));

//    centerRect  = new QGraphicsRectItem( nullptr );

    if( type == 0 ) {
        baseRect->setBrush(QColor(211,222,237,60));
    }

    this->grNmItem = new TGroupName( groupName );
    this->grNmItem->setParentItem(baseRect);

    loadTermNodes();

    checkSwapTimer.setSingleShot( false );
    setAnimSpeed( 300 );
    connect( &checkSwapTimer, SIGNAL( timeout() ),SLOT( checkSwap() ) );

    animTimer.setSingleShot(false);
    animTimer.setInterval(50);
    connect(&animTimer,SIGNAL(timeout()),SLOT(animateGroup()));

    animGrp.addAnimation(&swAnim1);
    animGrp.addAnimation(&swAnim2);

    swAnim1.setPropertyName("pos");
    swAnim2.setPropertyName("pos");
    swAnim1.setEasingCurve(QEasingCurve::InOutQuad);
    swAnim2.setEasingCurve(QEasingCurve::InOutQuad);
}

TermGroup::~TermGroup()
{
    checkSwapTimer.stop();
    animTimer.stop();

    delete grNmItem;
    delete treeRect;
    delete orphansRect;
    delete baseRect;
}

GroupType TermGroup::getType()
{
    return type;
}

QString TermGroup::getTypeString()
{
    return getTypesMap()[getType()];
}

QStringList TermGroup::getTypesNames()
{
    QMap<GroupType, QString> map = getTypesMap();
    QStringList ret;
    for( QString s : map )
        ret << s;

    return ret;
}

QMap<GroupType, QString> TermGroup::getTypesMap()
{
    QMap<GroupType, QString> ret;
    ret[freeEdges] = "Свободные связи";
    ret[terms]     = "Термины";
    return ret;
}

void TermGroup::loadTermNodes()
{
    nodeList.clear();

    QList< int > idLst = db->nodeTbl->getGroupNodeID( grUid );

    for(int id : idLst) {
        QSqlRecord rec = db->nodeTbl->getNode( id );

        if(rec.count() == 0)
            continue;

        TermNode *nd = new TermNode( rec );

        connect( nd, SIGNAL(startGroupAnimation()), SLOT(startAnimation()) );
        connect( nd, SIGNAL(stopGroupAnimation()),  SLOT(stopAnimation())  );

        nodeList << nd;
    }
}

void TermGroup::addNodesToParents()
{
    for(auto nd: getInTreeList()) {
        nd->setParentItem( treeRect );
    }

    for(auto nd: getOrphansList()) {
        nd->setParentItem( orphansRect );
    }
}

void TermGroup::swapNodes(TermNode *n1, TermNode *n2)
{
    if( animGrp.state() != QAbstractAnimation::Stopped )
        return; //Very important instruction.
    //Because of lags, timer can still work, but animation will slow down and it cause
    //animation lags

    currAnimLevel = n1->getPaintLevel();

    QRectF rc1 = n1->getMainRect(true);
    QRectF rc2 = n2->getMainRect(true);

    QPointF pos1, pos2;

    if( Glb::isVertical() ) {
        if(rc1.left() < rc2.left()) {
            pos1 = QPointF(n1->pos().x(),n2->pos().y());
            pos2 = QPointF(n2->pos().x()+rc2.width()-rc1.width(),n1->pos().y());
        } else {
            pos1 = QPointF(n1->pos().x()+rc1.width()-rc2.width(),n2->pos().y());
            pos2 = QPointF(n2->pos().x(),n1->pos().y());
        }
    } else {
        if(rc1.top() < rc2.top()) {
            pos1 = QPointF(n1->pos().x()+rc1.width()/2-rc2.width()/2,n1->pos().y());
            pos2 = QPointF(n2->pos().x()+rc2.width()/2-rc1.width()/2,n2->pos().y()-rc1.height()+rc2.height());
        } else {
            pos1 = QPointF(n1->pos().x()+rc1.width()/2-rc2.width()/2,n1->pos().y()-rc2.height()+rc1.height());
            pos2 = QPointF(n2->pos().x()+rc2.width()/2-rc1.width()/2,n2->pos().y());
        }
    }

    swAnim1.setTargetObject( n1 );
    swAnim1.setStartValue( n1->pos() );
    swAnim1.setEndValue  ( pos2 );

    swAnim2.setTargetObject( n2 );
    swAnim2.setStartValue( n2->pos() );
    swAnim2.setEndValue  ( pos1 );

    animGrp.start();
}

QJsonDocument TermGroup::getJsonDoc()
{
    QJsonDocument doc;
//    obj.insert("uid",     QJsonValue(grUid));
    QJsonObject obj;
    obj.insert( db->groupTbl->longUID, QJsonValue(this->longUid));
    obj.insert( db->groupTbl->name,    QJsonValue(getName()));
    obj.insert( db->groupTbl->type,    QJsonValue(this->type));

    QJsonArray ndArr;

    for( TermNode *n : nodeList ) {
        ndArr.append(n->toJson());
    }
    obj.insert( "nodesList", ndArr);

    doc.setObject( obj );
    return doc;
}

void TermGroup::startAnimation()
{
    animTimer.start();
    checkSwapTimer.start();
    grNmItem->timerStarted();
}

void TermGroup::stopAnimation()
{
    animTimer.stop();
    checkSwapTimer.stop();
    grNmItem->timerStopped();
}

void TermGroup::checkSwap()
{
    QTime t;
    t.start();
    int maxLevel = getAllLevelsCount();

    for( int i = 1; i <= maxLevel; i++ ) { //i=1 because we need to ignore roots
        NodesList levLst = getLevList(i);

        for( int j = 0; j < levLst.size() - 1; j++ ){

            int inter = levLst[j]->getIntersections();

            QPointF pt1 = levLst[j]->getCenter( );
            QPointF pt2 = levLst[j+1]->getCenter( );

            levLst[j]->setSwap(pt2);
            levLst[j+1]->setSwap(pt1);

            qreal sum1 = levLst[j]->getSumEdgesLength( false ) + levLst[j+1]->getSumEdgesLength( false );
            qreal sum2 = levLst[j]->getSumEdgesLength( true  ) + levLst[j+1]->getSumEdgesLength( true  );

            int newIntersect = levLst[j]->getIntersections( true );

            levLst[j]->dropSwap();
            levLst[j+1]->dropSwap();

            if( levLst[j]->getMainRect().intersects( levLst[j+1]->getMainRect() ) )
                continue;

            if( newIntersect < inter ) {
                swapNodes(levLst[j],levLst[j+1]);
                //                qDebug()<<"swapReason:intersections"<<t.elapsed();
                return;
            } else if( newIntersect <= inter && ( sum2 < sum1 && sum1-sum2 > sum1*0.2 ) ) {
                swapNodes(levLst[j],levLst[j+1]);
                //                qDebug()<<"swapReason:length"<<t.elapsed();
                return;
            }

        }
    }

    lockForce = false;

    //    qDebug()<<"noSwap"<<t.elapsed();
}

void TermGroup::animateGroup()
{
    //    if(animGrp.state() != QAbstractAnimation::Stopped)
    //        return;

    if( lockForce )
        return;

    for( TermNode * n : nodeList ) {
        if( animGrp.state() != QAbstractAnimation::Stopped && n->getPaintLevel() == currAnimLevel )
            continue;
        n->countForces();
    }


    bool someMoved = false;
    for( TermNode *n : nodeList ) {

        if( animGrp.state() != QAbstractAnimation::Stopped && n->getPaintLevel() == currAnimLevel )
            continue;

        if( n->applyMove() && !someMoved )
            someMoved = true;
    }


    if( !someMoved )
        animTimer.stop();

    if( Glb::isVertical() )
        centralizeTree();
}

NodesList TermGroup::getRootList()
{
    NodesList ret;
    for( TermNode *n : nodeList ) {
        if( n->isRoot() )
            ret << n;
    }
    return ret;
}

NodesList TermGroup::getLevList(int lev)
{
    NodesList ret;
    for( TermNode *t : nodeList )
        if( t->getPaintLevel() == lev )
            ret << t;

    //sort
    int nMin;

    if( Glb::isVertical() ) {
        for ( int i = 0; i < ret.size(); i++ ) {
            nMin = i;
            for ( int j = i+1; j < ret.size(); j++) {
                if( ret[j]->pos().x() < ret[nMin]->pos().x() ) {
                    nMin = j;
                }
            }
            ret.swap(i,nMin);
        }
    } else {
        for ( int i = 0; i < ret.size(); i++ ) {
            nMin = i;
            for ( int j = i+1; j < ret.size(); j++) {
                if( ret[j]->pos().y() < ret[nMin]->pos().y() ) {
                    nMin = j;
                }
            }
            ret.swap(i,nMin);
        }
    }
    return ret;
}

NodesList TermGroup::getOrphansList()
{
    NodesList ndLst;
    for( TermNode *n : nodeList )
        if( n->isOrphan() )
            ndLst << n;

    return ndLst;
}

bool TermGroup::hasOrphans() {
    return !getOrphansList().isEmpty();
}

NodesList TermGroup::getInTreeList()
{
    NodesList ret;
    for( TermNode *n : nodeList )
        if( n->isInTree() )
            ret << n;
    return ret;
}

qreal TermGroup::getSumSize(NodesList lst, bool withMargins, Qt::Orientation ori)
//Возвращает ширину списка вершин.
//По надобности суммирует интервалы между ними
{
    qreal ret = 0.0;

    if( ori == Qt::Vertical ) {
        for( TermNode *n : lst )
            if( withMargins )
                ret += n->getSize().width();
            else
                ret += n->getMainRect().width();
    } else {
        for( TermNode *n : lst )
            if( withMargins )
                ret += n->getSize().height();
            else
                ret += n->getMainRect().height();
    }

    return ret;
}

qreal TermGroup::getMaxSideSize( NodesList lst, Qt::Orientation ori )
{
    qreal ret = 0.0;

    if( ori == Qt::Vertical ) {
        for( TermNode *n : lst )
            ret = qMax( ret, n->getMainRect().height() );
    } else {
        for( TermNode *n : lst )
            ret = qMax( ret, n->getMainRect().width() );
    }

    return ret;
}

qreal TermGroup::getGroupMinWidth()
{
    qreal width = 0.0;

    width = qMax(width,getTitleMinWidth());
    width = qMax(width,getTreeMinWidth());
    width = qMax(width,getOrphansMinWidth());

    return width;
}

qreal TermGroup::getTitleMinWidth()
{
    return grNmItem->getNameRect().width();
}

qreal TermGroup::getOrphansMinWidth()
{
    qreal orphanMaxWidth = 0.0;
    for(auto orphan: getOrphansList()) {
        orphanMaxWidth = qMax(orphanMaxWidth, orphan->getMainRect().width());
    }
    return orphanMaxWidth;
}

qreal TermGroup::getTreeMinWidth()
{
    return getTreeSize().width();
}

NodesList TermGroup::getNodeList()
{
    return nodeList;
}

void TermGroup::setAnimSpeed(int val)
{
    checkSwapTimer.setInterval( val*1.5 );
    swAnim1.setDuration(val);
    swAnim2.setDuration(val);
    animSpeed = val;
}

int TermGroup::getAnimSpeed()
{
    return animSpeed;
}

void TermGroup::setBasePoint(QPointF pt)
{
    baseRect->setPos(pt);
}

void TermGroup::updateRectsPositions()
{
    qreal vSpacer = Sizes::groupVerticalSpacer;
    qreal hSpacer = Sizes::groupHorizontalSpacer;
    QPointF basePoint(QPointF(hSpacer, vSpacer));

    QSizeF nameSize = grNmItem->getNameRect().size();
    QSizeF treeSize = getTreeSize();
    QSizeF orphansSize = getOrphansSize();

    //Устанавливаем базовую точку имени
    grNmItem->setPos(basePoint);
    basePoint.ry() += nameSize.height() + vSpacer;

    //Вычисляем под дерево
    treeRect->setPos(basePoint);
    treeRect->setRect(QRectF(QPoint(), treeSize)); //Применяем его
    if (getTreeSize().height() > 0)
        basePoint.ry() += getTreeSize().height() + vSpacer;

    //Вычисляем под несвязанные вершины
    orphansRect->setPos(basePoint);
    orphansRect->setRect(QRectF(QPoint(), orphansSize)); //Применяем
}

void TermGroup::updateBaseRectSize()
{
    QSizeF nameSize = grNmItem->getNameRect().size();
    QSizeF treeSize = getTreeSize();
    QSizeF orphansSize = getOrphansSize();
    qreal vSpacer = Sizes::groupVerticalSpacer;
    qreal hSpacer = Sizes::groupHorizontalSpacer;

    qreal width = 0.0;
    qreal height = 0.0;

    width = qMax(width,nameSize.width());
    width = qMax(width,treeSize.width());
    width = qMax(width,orphansSize.width());
    width += hSpacer*2;

    height += vSpacer;
    height += nameSize.height();
    if (treeSize.height() > 0)
        height += treeSize.height() + vSpacer;
    if (orphansSize.height() > 0)
        height += orphansSize.height() + vSpacer;
    height += vSpacer;

    baseRect->setRect(QRectF(QPointF(),QSize(width,height)));
}

void TermGroup::updGroupFrame()
{
    updateRectsPositions();
    updateBaseRectSize();
}

void TermGroup::centralizeTree()
{
    NodesList lst = getInTreeList();

    QRectF rc = getAllGroupRect( false );
    QPointF correction;
    correction += baseRect->scenePos();
    correction -= rc.center();
    correction.setY(0);

    for( TermNode* n : lst )
        n->setPos( n->pos() + correction );

}

EdgesList TermGroup::searchConnections()
{
    EdgesList ret;
    for( TermNode *n : nodeList ) {
        for( TermNode *m : nodeList) {
            if( n == m )
                continue;
            QStringList lst = n->getTags();
            for( QString s : lst ) {
                for( QString z : m->getNameFormList() ) {
                    QString s1 = z.toLower();
                    QString s2 = s.toLower();
                    if( s1 == s2 ) //Точное соответствие
                        ret << new Edge( m, n );
                    int acceptableDistance = 4*( 1 + s1.count(' ') ); //Пропорционально количеству слов
                    acceptableDistance = 4;
                    //TODO: Сделать защиту от формирования двухсторонних связей
                    //TODO: Найти способ вызывать функцию в mainScene addEdge
                    //TODO: Переделать так чтобы это было предложением а не обязательным действием
                    if( Glb::getLevDistance( s1, s2 ) <= acceptableDistance )
                        if( s1.left(3) == s2.left(3) )
                            ret << new Edge( m, n );
                }

            }
        }
    }
    return ret;
}

int TermGroup::getAllLevelsCount() const
{
    int ret = 0;
    for( TermNode *t : nodeList )
        ret = qMax( ret, t->getPaintLevel() );

    return ret;
}

void TermGroup::setOrphCoords()
{
    QPointF startPoint; // = orphansRect->rect().topLeft();

//    if( hasTree() ) {
//        QRectF treeRc = getAllGroupRect( false );
//        startPoint.setY( treeRc.bottom() + 20 );
//    }

    NodesList ndLst = getOrphansList();

    if( ndLst.isEmpty() )
        return;

    qDebug()<<ndLst.count();
    for(auto nd: ndLst) {
        if (nd->parentItem() == orphansRect)
            qDebug()<<"good parent";
    }

    qreal maxSide = getMaxSideSize( ndLst );

    qreal maxWidth = 700.0;

    int x = startPoint.x();
    int y = startPoint.y();
    qreal tmpWidth = 0.0;
    int from = 0;

    for( int i = 0; i < ndLst.size(); i++ ) {
        ndLst[i]->setPos( x, y );
        tmpWidth += ndLst[i]->getSize().width();
        x += ndLst[i]->getSize().width();

        if( x > maxWidth || i == ( ndLst.size() - 1 ) ) {
            x = 0;
            y+= maxSide + 15;
//            for(int j=from;j<=i;j++){
//                ndLst[j]->moveBy((maxWidth-tmpWidth)/2,0); //TODO: Подумать над этим моментом
//            }
            tmpWidth = 0.0;
            from = i+1;
        }
    }
}

void TermGroup::setLevels()
{
    NodesList rList = getRootList();
    for( TermNode *t : rList )
        t->setLevel(0);

}

void TermGroup::setTreeCoords()
{
    int maxLevel = getAllLevelsCount();

    if( nodeList.isEmpty() )
        return;

    int x = 0, y = 0;
    int sumSize = 0;
    qreal maxLevelSide = 0;

    for(int i = 0; i <= maxLevel; i++) {
        QList< TermNode *> tList = getLevList(i);

        int nMax = 0;

        for(int j=0; j< tList.size(); j++ ){
            nMax = j;
            for(int k = j+1; k < tList.size(); k++ ){
                if( tList[k]->getUpLevels() > tList[nMax]->getUpLevels() )
                    nMax = k;
            }
            tList.swap(j,nMax);
        }

        QList< TermNode* > tmpList;

        for( int j = 0; j < tList.size(); j++ ) {
            if( j % 2 == 0 )
                tmpList.push_front(tList[j]);
            else
                tmpList.push_back(tList[j]);
        }

        tList = tmpList;

        sumSize       =   getSumSize( tList, false, Glb::ori );
        maxLevelSide = getMaxSideSize( tList, Glb::ori );
        if( Glb::isVertical() )
            x = -sumSize/2;
        else
            y = sumSize/2;

        if( Glb::isVertical() ) //Сначала добавляем первую половину максимума
            y += ( maxLevelSide/2 );
        else
            x += ( maxLevelSide/2 );


        for( TermNode *n : tList ) {
            if( Glb::isVertical() ) {
                n->setPos( x , -y - n->getMainRect().height()/2 );
                x += n->getSize().width();
            } else {
                n->setPos( x - n->getMainRect().width()/2, -y );
                y -= n->getSize().height();
                if( getGroupUid() == 4 && i == 0){
//                    qDebug()<<"size"<<n->getName()<<n->getSize();
                }
            }
        }

        if( Glb::isVertical() ) //А после выставления всех координат - вторую
            y += ( maxLevelSide/2 + TermNode::vInterv );
        else
            x += ( maxLevelSide/2 + TermNode::hInterv );

    }

}

void TermGroup::setNeighbours()
{
    QList<int> levLst;

    for( TermNode *n : nodeList ) {
        if( !levLst.contains( n->getPaintLevel() ) )
            levLst << n->getPaintLevel();
    }

    QList<TermNode *> levNd;
    for( int i : levLst ) {
        levNd.clear();
        for( TermNode *n : nodeList ) {
            if( n->getPaintLevel() == i ) {
                levNd << n;
            }
        }
        for( int j = 0; j< levNd.size(); j++ ) {
            levNd[j]->clearNeighbours();
            for( int k = 0; k < levNd.size(); k++ ) {
                if(j == k)
                    continue;
                levNd[j]->addToNeighboursList( levNd[k] );
            }
        }
    }
}

QString TermGroup::getName()
{
    return grNmItem->getNameOnly();
}

int TermGroup::getGroupUid()
{
    return grUid;
}

QRectF TermGroup::getAllGroupRect( bool withOrph )
{
    NodesList nodes;

    if( nodeList.isEmpty() )
        return QRectF(baseRect->pos(),baseRect->rect().size());

    if( withOrph )
        nodes = nodeList;
    else
        nodes = getRootList();

    QRectF rc;
    for( TermNode *t : nodes ) {
        if( rc.isNull() )
            rc = t->getMainRect();

        rc = rc.united( t->getTreeSize(rc) );
    }

    QRectF strRect = grNmItem->getNameRect();

    rc.setWidth(  qMax( rc.width(), strRect.width()  ) );
    rc.setHeight( qMax( rc.height(),strRect.height() ) );

    return rc;
}

QSizeF TermGroup::getOrphansSize()
{
    QRectF orphansRc;
    for(auto nd: getOrphansList()) {
        orphansRc = orphansRc.united(nd->getMainRect());
    }
    return orphansRc.size();
}

bool TermGroup::hasTree()
{
    for( TermNode *n : nodeList ) {
        if( n->isInTree() )
            return true;
    }
    return false;
}

QSizeF TermGroup::getTreeSize()
{
    int levels = getAllLevelsCount();
    qreal sumWidth = 0.0;
    qreal maxHeight = 0.0;

    for(int level = 0; level <= levels; level++) {
        qreal maxLevelWidth = 0.0;
        qreal sumHeight = 0.0;
        for(auto node: getLevList(level)) {
            QRectF nodeRect = node->getMainRect();
            maxLevelWidth = qMax(maxLevelWidth, nodeRect.width());
            sumHeight += nodeRect.height();
        }
        sumWidth += maxLevelWidth;
        maxHeight = qMax(maxHeight, sumHeight);
    }
    return QSizeF(sumWidth,maxHeight);
}
