#include "termnode.h"

bool TermNode::someoneHover  = false;
bool TermNode::someoneSelect = false;

const qreal TermNode::verScale = 0.0200;

QList< Qt::Edge > TermNode::sides;

TermNode::TermNode(QSqlRecord rec):
    TermGraph(rec),
    QGraphicsItem()
{
    if (sides.isEmpty()) {
        sides << Qt::BottomEdge;
        sides << Qt::TopEdge;
        sides << Qt::RightEdge;
        sides << Qt::LeftEdge;
    }

    adjustSizeForName();

    //    setFlag( QGraphicsItem::ItemIsSelectable,true );
    //    setFlag(QGraphicsItem::ItemIsMovable,false);

    setAcceptHoverEvents(true);
    setZValue(1);
}

TermNode::~TermNode()
{
}

void TermNode::adjustSizeForName()
{
    prepareGeometryChange();
    QSizeF nameSize = getNameSize();
    nodeSize.setWidth(nameSize.width() + 16);
    nodeSize.setHeight(nameSize.height() + 4);
}

TermNode *TermNode::getNearestLeftNeigh()
{
    TermNode* ret = nullptr;
    qreal diff = 100000.0;

    QRectF myRect = getNodeRect(CoordType::scene);
    QRectF neighRect;

    for (TermNode *t : neighbourNodes) {
        neighRect = t->getNodeRect(CoordType::scene);

        if (neighRect.center().x() > myRect.center().x())
            continue;

        if (qAbs(neighRect.center().x() - myRect.center().x()) < diff) {
            diff = qAbs(neighRect.center().x() - myRect.center().x());
            ret = t;
        }
    }
    return ret;
}

TermNode *TermNode::getNearestRightNeigh()
{
    TermNode* ret = nullptr;
    qreal diff = 100000.0;

    QRectF myRect = getNodeRect(CoordType::scene);
    QRectF neighRect;
    for (TermNode *t : neighbourNodes) {
        neighRect = t->getNodeRect(CoordType::scene);

        if (neighRect.center().x() < myRect.center().x())
            continue;

        if (qAbs(neighRect.left() - myRect.right()) < diff) {
            diff = qAbs(neighRect.left() - myRect.right());
            ret = t;
        }
    }
    return ret;
}

EdgesList TermNode::getEdgesInLayer()
{
    EdgesList ret;
    for (TermNode *t : neighbourNodes) {
        ret << t->edgesToRoots;
        ret << t->edgesToLeafs;
    }

    EdgesList ret2;
    for (Edge *e : ret) {
        if (e->getLayerDistance() == 1)
            ret2 << e;
    }
    return ret2;
}

QRectF TermNode::getNodeRect(CoordType inCoordinates) const
{
    QRectF ret = getInnerNodeRect();

    switch (inCoordinates) {
    case CoordType::none: break;
    case CoordType::local:
        ret = ret.translated(this->pos());
        break;
    case CoordType::scene:
        ret = ret.translated(this->scenePos());
        break;
    }

    return ret;
}

QPointF TermNode::getCenter(CoordType inCoordinates) const
{
    return getNodeRect(inCoordinates).center();
}

QRectF TermNode::boundingRect() const
{
    return getInnerNodeRect();
}

QRectF TermNode::getRcWithBorders()
{
    //    qreal val = mainRect.width()*0.08;
    qreal val = qBound(0.0, nodeSize.width()*0.1, 8.0);
    QMarginsF mrg(val, val, val, val);
    QRectF ret = getInnerNodeRect();
    ret = ret.marginsAdded(mrg);
    ret = ret.translated(this->scenePos());
    return ret;
}

QSizeF TermNode::getSize(bool withBorder)
{
    qreal val = qBound(0.0, nodeSize.height()*0.2, 8.0);
    QRectF ret = getInnerNodeRect();
    if (withBorder) {
        QMarginsF mrg(val, val, val, val);
        ret = ret.marginsAdded(mrg);
    }
    return ret.size();
}

QLineF TermNode::getRectLine(Qt::Edge side)
{
    QRectF rc = getNodeRect(CoordType::local);

    switch (side) {
    case Qt::TopEdge:
        return QLineF(rc.topLeft(), rc.topRight());

    case Qt::RightEdge:
        return QLineF(rc.topRight(), rc.bottomRight());

    case Qt::BottomEdge:
        return QLineF(rc.bottomLeft(), rc.bottomRight());

    case Qt::LeftEdge:
        return QLineF(rc.topLeft(), rc.bottomLeft());
    }
    return QLineF();
}

NodeType TermNode::getNodeType()
{
    if (edgesToRoots.isEmpty()) {
        if (edgesToLeafs.isEmpty()) {
            return NodeType::orphan;      // Оба пустые
        } else {
            return NodeType::root;        // Вниз связей нет, вверх - есть
        }
    } else {
        if (edgesToLeafs.isEmpty()) {
            return NodeType::endLeaf;     // Вниз есть, а вверх - нету
        } else {
            return NodeType::middleLeaf;  // Есть и вверх и вниз
        }
    }
}

QColor TermNode::getBaseColor()
{
    switch (getNodeType()) {
    case NodeType::orphan: return AppStyle::Colors::Nodes::orphan;
    case NodeType::root: return AppStyle::Colors::Nodes::root;
    case NodeType::endLeaf: return AppStyle::Colors::Nodes::leaf;
    case NodeType::middleLeaf: return AppStyle::Colors::Nodes::leaf;
    }
}

QColor TermNode::getSelectedColor()
{
    switch (getNodeType()) {
    case NodeType::orphan: return AppStyle::Colors::Nodes::orphanSelected;
    case NodeType::root: return AppStyle::Colors::Nodes::rootSelected;
    case NodeType::endLeaf: return AppStyle::Colors::Nodes::leafSelected;
    case NodeType::middleLeaf: return AppStyle::Colors::Nodes::leafSelected;
    }
}

void TermNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // defaults
    QRectF rcBase = getInnerNodeRect();
    QColor col = getBaseColor();
    int transpVal = 100;
    setZValue(1);

    if (someoneHover || someoneSelect) {
        if (thisHovered || isSelected() || relative) {
            if (isSelected() || thisHovered)
                col = getSelectedColor();
            col.setAlpha(255);
            setZValue(2);
        } else {
            col.setAlpha(transpVal);
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

    QBrush br(col, Qt::SolidPattern);

    QColor borderColor = AppStyle::Colors::Nodes::border;
    borderColor.setAlpha(col.alpha());
    painter->setPen(borderColor);
    painter->setBrush(br);

    qreal minSide = qMin(rcBase.width(), rcBase.height());
    int cornerRadius = qRound(minSide * 0.18);
    if (thisHovered)
        painter->drawRoundedRect(rcBase.adjusted(-1, -1, 1, 1), cornerRadius, cornerRadius);
    else
        painter->drawRoundedRect(rcBase, cornerRadius, cornerRadius);

    painter->setPen(AppStyle::Colors::Nodes::text);
    painter->drawText(rcBase.adjusted(1, 0, 1, 0), Qt::AlignCenter, str);

    if (atLearning()) {
        QRectF miniRc = QRectF(rcBase.topLeft(), QSizeF(11, 14));
        painter->setBrush(AppStyle::Colors::Nodes::atLearn);
        painter->drawRoundedRect(miniRc, 1, 1);
        painter->drawText(rcBase.topLeft() + QPointF(2, 12), QString::number(getRepNum()));
    }
}

void TermNode::setRelatPaint(bool val)
{
    for (TermGraph* n : getRootNodes()) {
        dynamic_cast<TermNode*>(n)->relative = val;
    }

    for (TermGraph* n : getLeafNodes()) {
        dynamic_cast<TermNode*>(n)->relative = val;
    }

    for (Edge *e : edgesToRoots)
        e->wide = val;

    for (Edge *e : edgesToLeafs)
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

    if (!isSelected())
        setRelatPaint(false);

    QGraphicsItem::hoverLeaveEvent(evt);
}

void TermNode::mousePressEvent(QGraphicsSceneMouseEvent *evt)
{
    QGraphicsItem::mousePressEvent(evt);
}

bool TermNode::hasConnections()
{
    return !( getRootNodes().isEmpty() && getLeafNodes().isEmpty() );
}

bool TermNode::hasConnectionsInGroup()
{
    for (Edge *e : edgesToLeafs) {
        if (e->getLeaf()->getGroupUuid() == getGroupUuid()) {
            return true;
        }
    }

    for (Edge *e : edgesToRoots) {
        if (e->getLeaf()->getGroupUuid() == getGroupUuid()) {
            return true;
        }
    }
    return false;
}

void TermNode::setLevel(int lev)
{
    if (lev > paintLevel)
        paintLevel = lev;

    for (TermGraph *t : getLeafNodes()) {
        TermNode* n = dynamic_cast<TermNode*>(t);
        if (getGroupUuid() != n->getGroupUuid())
            continue;
        n->setLevel(lev + 1);
    }
}

int TermNode::getUpLevels(int pLevel)
{
    Q_UNUSED(pLevel)  // TODO: check!
    int ret = -1;
    for (TermGraph* t : getLeafNodes()) {
        TermNode* n = dynamic_cast<TermNode*>(t);
        ret = qMax(ret, n->getUpLevels());
    }

    if (ret == -1)
        ret = getPaintLevel();

    return ret;
}

void TermNode::clearNeighbours()
{
    neighbourNodes.clear();
}

void TermNode::clearConnBrRootLists()
{
    clearRootNodes();
    clearLeafNodes();
}

void TermNode::addToNeighboursList(TermNode *t)
{
    neighbourNodes << t;
}

void TermNode::addEdgeRef(Edge *edge)
{
    if (edge->getRoot() == this && edge->getLeaf() != this) {  // We are source - connection up
        edgesToLeafs    << edge;
        addNodeToLeafNodes(edge->getLeaf());
    }

    if (edge->getLeaf() == this && edge->getRoot() != this) {  // We are acceptor - connection down
        edgesToRoots << edge;
        addNodeToRootNodes(edge->getRoot());
    }
}

int TermNode::getPaintLevel() { return paintLevel; }

void TermNode::countForces()
{
    if (!hasConnections() && !isRoot())
        return;

    qreal edForce = 0.0;
    qreal myX = getCenter(CoordType::scene).y();

    qreal tmp = 0.0;
    qreal notMyPos = 0.0;

    EdgesList edges;
    edges << edgesToRoots;
    edges << edgesToLeafs;

    for (Edge *e : edges) {
        if (!e->isInGroupEdge())
            continue;

        tmp = e->getYProjection();

        if (e->getRoot() == this)
            notMyPos = e->getLeaf()->getCenter(CoordType::scene).y();
        else if (e->getLeaf() == this)
            notMyPos = e->getRoot()->getCenter(CoordType::scene).y();
        else
            continue;

        tmp *= verScale;

        if (notMyPos > myX)
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

    for (int i = 0; i < edges.size(); i++) {
        for (int j = i+1; j < edges.size(); j++) {
            QLineF l1 = edges[i]->getLine(swapped),
                    l2 = edges[j]->getLine(swapped);

            QPointF *interPt = new QPointF();
            if (l1.intersect(l2, interPt) == QLineF::BoundedIntersection) {
                bool nearFound = false;

                qreal dist = 0.01;
                if (isNearPoints(*interPt, l1.p1(), dist))
                    nearFound = true;
                if (isNearPoints(*interPt, l1.p2(), dist))
                    nearFound = true;
                if (isNearPoints(*interPt, l2.p1(), dist))
                    nearFound = true;
                if (isNearPoints(*interPt, l2.p2(), dist))
                    nearFound = true;

                if (!nearFound) {
                    bool interFound = false;
                    for (QPointF inPt : interList) {
                        if (isNearPoints(inPt, *interPt, dist)) {
                            interFound = true;
                            break;
                        }
                    }
                    if (!interFound)
                        interList << *interPt;
                }
            }
            delete interPt;
        }
    }
    return interList.size();
}

qreal TermNode::getSumEdgesLength(bool swap = false)
{
    EdgesList edges;
    edges << edgesToLeafs;
    edges << edgesToRoots;
    qreal ret = 0.0;
    for (Edge *e : edges) {
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

    for (Edge *e : edgesToRoots) {
        e->swPtBran = toPt;
    }

    for (Edge *e : edgesToLeafs) {
        e->swPtRoot = toPt;
    }
}

void TermNode::dropSwap()
{
    EdgesList lst;
    lst = getConnectedEdges();

    for (Edge *e : lst) {
        e->swPtBran = QPointF();
        e->swPtRoot = QPointF();
    }
}

bool TermNode::isNearPoints(QPointF pt1, QPointF pt2, qreal dist) {
    pt1 -= pt2;
    //    pt1.setX(qAbs(pt1.x()));
    //    pt1.setY(qAbs(pt1.y()));
    //    if(qAbs(pt1.x()) <= dist && qAbs(pt1.y()) <= dist)
    //        return true;
    if (pt1.manhattanLength() <= dist)
        return true;
    return false;
}

QRectF TermNode::getInnerNodeRect() const
{
    return QRectF(QPointF(0.0, 0.0), nodeSize);
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

bool TermNode::applyMove()
{
    // Если сильно смещение - кидаем true
    TermNode *left = getNearestLeftNeigh();
    TermNode *right = getNearestRightNeigh();

    QRectF myRc = getRcWithBorders();
    myRc = myRc.translated(newPosOffs, 0.0);
    QRectF leftRc;
    QRectF rightRc;

    qreal k = 0.01;
    newPosOffs = qBound(-myRc.width()*k, newPosOffs, myRc.width()*k);

    if (newPosOffs < 0) {
        if (left != nullptr) {
            leftRc = left->getRcWithBorders();
            newPosOffs = qBound(leftRc.right() - myRc.left(), newPosOffs, 0.0);
        }
    }

    if (newPosOffs > 0) {
        if (right != nullptr) {
            rightRc = right->getRcWithBorders();
            newPosOffs = qBound(0.0, newPosOffs, rightRc.left() - myRc.right());
        }
    }


    if (qAbs(newPosOffs) > 0.05) {
        moveBy(0.0, newPosOffs);
        return true;
    }

    return false;
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

    p << Glb::ptToStr(pos());

    tmp = " childs:";
    QList<QGraphicsItem*> childs = childItems();
    for (int i = 0; i < childs.size(); i++) {
        TermNode *n = (TermNode*)childs[i];
        tmp += " " + n->getUuid().toString();
    }
    p << Glb::ptToStr(scenePos());

    QString addInterf;
    addInterf += QString::number(getIntersections()) + " ";
    addInterf += QString::number(getIntersections(true));
    p << addInterf;

    p << " upLevels:" + QString::number(getUpLevels() - getPaintLevel());
    //        addInterf += QString::number(personalAddInterval) + " ";
    p << "Uuid" << getUuid().toString();
    return p.join("\n");
}
