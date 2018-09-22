#include "paintednode.h"

bool PaintedNode::someoneHover  = false;
bool PaintedNode::someoneSelect = false;

const qreal PaintedNode::verScale = 0.0200;
QList<Qt::Edge> PaintedNode::sides;

PaintedNode::PaintedNode(QSqlRecord rec) :
    GraphTerm (rec)
{
    if (sides.isEmpty()) {
        sides << Qt::BottomEdge;
        sides << Qt::TopEdge;
        sides << Qt::RightEdge;
        sides << Qt::LeftEdge;
    }
}

int PaintedNode::getUpLevels(int pLevel)
{
    Q_UNUSED(pLevel)  // TODO: check!
    int ret = -1;
    for (GraphTerm* t : getLeafNodes()) {
        PaintedNode* n = static_cast<PaintedNode*>(t);
        ret = qMax(ret, n->getUpLevels());
    }

    if (ret == -1)
        ret = getPaintLevel();

    return ret;
}

void PaintedNode::setRelatedPaint(bool val)
{
    for (GraphTerm* n : getUpDownNodes()) {
        static_cast<PaintedNode*>(n)->relativePaint = val;
    }

    for (GraphEdge* d : getUpDownEdges()) {
        auto e = dynamic_cast<Edge*>(d);
        e->setWide(val);
    }
}

QLineF PaintedNode::getRectLine(Qt::Edge side)
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

QRectF PaintedNode::getNodeRect(CoordType inCoordinates) const
{
    QRectF ret = QRectF(QPointF(), nodeSize);

    switch (inCoordinates) {
    case CoordType::zeroPoint: break;
    case CoordType::local:
        ret = ret.translated(this->getPos());
        break;
    case CoordType::scene:
        ret = ret.translated(this->getScenePos());
        break;
    }

    return ret;
}

QRectF PaintedNode::getFrameRect(CoordType inCoordinates) const
{
    qreal val = qBound(0.0, nodeSize.width()*0.2, 8.0);
    QRectF ret = getNodeRect(inCoordinates);
    return addMarginsToRect(ret, val);
}

QPointF PaintedNode::getCenter(CoordType inCoordinates) const
{
    return getNodeRect(inCoordinates).center();
}

bool PaintedNode::applyMove()
{
    // Если сильно смещение - кидаем true
    PaintedNode* left = getNearestLeftNeigh();
    PaintedNode* right = getNearestRightNeigh();

    QRectF myRc = getFrameRect(CoordType::scene);
    myRc = myRc.translated(newPosOffset, 0.0);
    QRectF leftRc;
    QRectF rightRc;

    qreal k = 0.01;
    newPosOffset = qBound(-myRc.width()*k, newPosOffset, myRc.width()*k);

    if (newPosOffset < 0) {
        if (left != nullptr) {
            leftRc = left->getFrameRect(CoordType::scene);
            newPosOffset = qBound(leftRc.right() - myRc.left(), newPosOffset, 0.0);
        }
    }

    if (newPosOffset > 0) {
        if (right != nullptr) {
            rightRc = right->getFrameRect(CoordType::scene);
            newPosOffset = qBound(0.0, newPosOffset, rightRc.left() - myRc.right());
        }
    }

    if (qAbs(newPosOffset) > 0.05) {
        movePosBy(0.0, newPosOffset);
        return true;
    }

    return false;
}

void PaintedNode::countForces()
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

        PaintedNode* otherSide = dynamic_cast<PaintedNode*>(e->getOtherSide(this));
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

    newPosOffset = 0.0;
    newPosOffset += edForce;
}

int PaintedNode::getIntersections(bool swapped)
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

EdgesList PaintedNode::getEdgesInLayer()
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

QRectF PaintedNode::addMarginsToRect(QRectF rc, qreal mrg)
{
    QMarginsF mrgObj(mrg, mrg, mrg, mrg);
    return rc.marginsAdded(mrgObj);
}

void PaintedNode::dropSwap()
{
    EdgesList lst;
    lst = Edge::castToEdgeList(getUpDownEdges());

    for (Edge *e : lst) {
        e->swapPointLeaf = QPointF();
        e->swapPointRoot = QPointF();
    }
}

void PaintedNode::setSwap(QPointF toPt)
{
    for (Edge *e : Edge::castToEdgeList(getEdgesToRoots())) {
        e->swapPointLeaf = toPt;
    }

    for (Edge *e : Edge::castToEdgeList(getEdgesToLeafs())) {
        e->swapPointRoot = toPt;
    }
}

qreal PaintedNode::getSumEdgesLength(bool swap)
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

void PaintedNode::adjustRectSizeForName()
{
    PrepareGeometryChangeCall();
    QSizeF nameSize = getNameSize();
    nodeSize.setWidth(nameSize.width() + 16);
    nodeSize.setHeight(nameSize.height() + 4);
}

PaintedNode* PaintedNode::getNearestLeftNeigh()
{
    PaintedNode* ret = nullptr;
    qreal diff = 100000.0;

    QRectF myRect = getNodeRect(CoordType::scene);
    QRectF neighRect;

    for (GraphTerm* n : getNeighbourNodes()) {
        PaintedNode* t = static_cast<PaintedNode*>(n);
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

PaintedNode *PaintedNode::getNearestRightNeigh()
{
    PaintedNode* ret = nullptr;
    qreal diff = 100000.0;

    QRectF myRect = getNodeRect(CoordType::scene);
    QRectF neighRect;
    for (GraphTerm* n : getNeighbourNodes()) {
        PaintedNode* t = static_cast<PaintedNode*>(n);
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

QColor PaintedNode::getBaseColor()
{
    switch (getNodeType()) {
    case NodeType::orphan: return AppStyle::Colors::Nodes::orphan;
    case NodeType::root: return AppStyle::Colors::Nodes::root;
    case NodeType::endLeaf: return AppStyle::Colors::Nodes::leaf;
    case NodeType::middleLeaf: return AppStyle::Colors::Nodes::leaf;
    }
}

QColor PaintedNode::getSelectedColor()
{
    switch (getNodeType()) {
    case NodeType::orphan: return AppStyle::Colors::Nodes::orphanSelected;
    case NodeType::root: return AppStyle::Colors::Nodes::rootSelected;
    case NodeType::endLeaf: return AppStyle::Colors::Nodes::leafSelected;
    case NodeType::middleLeaf: return AppStyle::Colors::Nodes::leafSelected;
    }
}

bool PaintedNode::isNearPoints(QPointF pt1, QPointF pt2, qreal dist) {
    pt1 -= pt2;
    //    pt1.setX(qAbs(pt1.x()));
    //    pt1.setY(qAbs(pt1.y()));
    //    if(qAbs(pt1.x()) <= dist && qAbs(pt1.y()) <= dist)
    //        return true;
    if (pt1.manhattanLength() <= dist)
        return true;
    return false;
}
