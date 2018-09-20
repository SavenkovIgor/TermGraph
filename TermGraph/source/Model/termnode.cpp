#include "termnode.h"

const qreal TermNode::verScale = 0.0200;

QList< Qt::Edge > TermNode::sides;

TermNode::TermNode(QSqlRecord rec):
    PaintedNode(rec),
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

    for (GraphTerm* n : getNeighbourNodes()) {
        TermNode* t = dynamic_cast<TermNode*>(n);
        neighRect = t->getNodeRect(CoordType::scene);

        if (neighRect.center().x() > myRect.center().x()) {
            continue;
        }

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
    for (GraphTerm* n : getNeighbourNodes()) {
        TermNode* t = dynamic_cast<TermNode*>(n);
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
    // Taking all edges in this paint level
    GraphEdge::List allEdgesInLayerList;
    for (GraphTerm* t : getNeighbourNodes()) {
        allEdgesInLayerList << t->getEdgesToRoots();
        allEdgesInLayerList << t->getEdgesToLeafs();
    }

    // Stay only with distance 1
    EdgesList edgLst = Edge::castToEdgeList(allEdgesInLayerList);
    EdgesList ret2;
    for (Edge *e : edgLst) {
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

void TermNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // defaults
    QRectF rcBase = getInnerNodeRect();
    QColor col = getBaseColor();
    int transpVal = 100;
    setZValue(1);

    if (someoneHover || someoneSelect) {
        if (thisHovered || isSelected() || relativePaint) {
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

void TermNode::setRelatedPaint(bool val)
{
    for (GraphTerm* n : getUpDownNodes()) {
        dynamic_cast<TermNode*>(n)->relativePaint = val;
    }

    for (GraphEdge* d : getUpDownEdges()) {
        auto e = dynamic_cast<Edge*>(d);
        e->setWide(val);
    }
}

void TermNode::hoverEnterEvent(QGraphicsSceneHoverEvent *evt)
{
    someoneHover = true;
    thisHovered = true;

    relativePaint = true;

    setRelatedPaint(true);

    QGraphicsItem::hoverEnterEvent(evt);
}

void TermNode::hoverLeaveEvent(QGraphicsSceneHoverEvent *evt)
{
    thisHovered = false;
    someoneHover = false;

    relativePaint = false;

    if (!isSelected())
        setRelatedPaint(false);

    QGraphicsItem::hoverLeaveEvent(evt);
}

void TermNode::mousePressEvent(QGraphicsSceneMouseEvent *evt)
{
    QGraphicsItem::mousePressEvent(evt);
}

void TermNode::countForces()
{
    if (!hasConnections() && !isRoot())
        return;

    qreal edForce = 0.0;
    qreal myX = getCenter(CoordType::scene).y();

    qreal tmp = 0.0;
    qreal notMyPos = 0.0;

    EdgesList edges;
    edges << Edge::castToEdgeList(getEdgesToRoots());
    edges << Edge::castToEdgeList(getEdgesToLeafs());

    for (Edge *e : edges) {
        if (!GraphTerm::isInGroupEdge(e))
            continue;

        tmp = e->getYProjection();

        TermNode* otherSide = dynamic_cast<TermNode*>(e->getOtherSide(this));
        if (otherSide == nullptr) {
            continue;
        }

        notMyPos = otherSide->getCenter(CoordType::scene).y();

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
    edges << Edge::castToEdgeList(getEdgesToRoots());

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
    edges << Edge::castToEdgeList(getEdgesToLeafs());
    edges << Edge::castToEdgeList(getEdgesToRoots());
    qreal ret = 0.0;
    for (Edge *e : edges) {
        if (!GraphTerm::isInGroupEdge(e))
            continue;
        ret += qAbs(e->getLine(swap).dx());
    }
    return ret;
}

void TermNode::setSwap(QPointF toPt)
{
    for (Edge *e : Edge::castToEdgeList(getEdgesToRoots())) {
        e->swapPointLeaf = toPt;
    }

    for (Edge *e : Edge::castToEdgeList(getEdgesToLeafs())) {
        e->swapPointRoot = toPt;
    }
}

void TermNode::dropSwap()
{
    EdgesList lst;
    lst = Edge::castToEdgeList(getUpDownEdges());

    for (Edge *e : lst) {
        e->swapPointLeaf = QPointF();
        e->swapPointRoot = QPointF();
    }
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

QString TermNode::getDebugString() {
    QStringList p;
    QString tmp;

    p << HelpStuff::ptToStr(pos());

    tmp = " childs:";
    QList<QGraphicsItem*> childs = childItems();
    for (int i = 0; i < childs.size(); i++) {
        TermNode* n = dynamic_cast<TermNode*>(childs[i]);
        tmp += " " + n->getUuid().toString();
    }
    p << HelpStuff::ptToStr(scenePos());

    QString addInterf;
    addInterf += QString::number(getIntersections()) + " ";
    addInterf += QString::number(getIntersections(true));
    p << addInterf;

    p << " upLevels:" + QString::number(getUpLevels() - getPaintLevel());
    //        addInterf += QString::number(personalAddInterval) + " ";
    p << "Uuid" << getUuid().toString();
    return p.join("\n");
}
