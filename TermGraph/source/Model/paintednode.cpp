#include "paintednode.h"

bool PaintedNode::someoneHover  = false;
bool PaintedNode::someoneSelect = false;

PaintedNode::PaintedNode(QSqlRecord rec) :
    GraphTerm (rec)
{

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
    QRectF ret = getInnerNodeRect();

    switch (inCoordinates) {
    case CoordType::none: break;
    case CoordType::local:
        ret = ret.translated(this->getPos());
        break;
    case CoordType::scene:
        ret = ret.translated(this->getScenePos());
        break;
    }

    return ret;
}

QPointF PaintedNode::getCenter(CoordType inCoordinates) const
{
    return getNodeRect(inCoordinates).center();
}

QRectF PaintedNode::getRcWithBorders() const
{
    //    qreal val = mainRect.width()*0.08;
    qreal val = qBound(0.0, nodeSize.width()*0.1, 8.0);
    QMarginsF mrg(val, val, val, val);
    QRectF ret = getInnerNodeRect();
    ret = ret.marginsAdded(mrg);
    ret = ret.translated(getScenePos());
    return ret;
}

QSizeF PaintedNode::getSize(bool withBorder) const
{
    qreal val = qBound(0.0, nodeSize.height()*0.2, 8.0);
    QRectF ret = getInnerNodeRect();
    if (withBorder) {
        QMarginsF mrg(val, val, val, val);
        ret = ret.marginsAdded(mrg);
    }
    return ret.size();
}

bool PaintedNode::applyMove()
{
    // Если сильно смещение - кидаем true
    PaintedNode* left = getNearestLeftNeigh();
    PaintedNode* right = getNearestRightNeigh();

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
        movePosBy(0.0, newPosOffs);
        return true;
    }

    return false;
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

/*
void PaintedNode::setRelatPaint(bool val)
{
    for (GraphTerm* n : getUpDownNodes()) {
        static_cast<PaintedNode*>(n)->relative = val;
    }

    for (GraphEdge* d : getUpDownEdges()) {
        auto e = static_cast<Edge*>(d);
        e->wide = val;
    }
}
*/
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
/*
EdgesList PaintedNode::getEdgesInLayer()
{
    GraphEdge::List ret;
    for (GraphTerm* t : getNeighbourNodes()) {
        ret << t->getEdgesToRoots();
        ret << t->getEdgesToLeafs();
    }

    EdgesList srcList = Edge::castToEdgeList(ret);
    EdgesList ret2;
    for (Edge* e : srcList) {
        if (e->getLayerDistance() == 1)
            ret2 << e;
    }
    return ret2;
}
*/
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

QRectF PaintedNode::getInnerNodeRect() const
{
    return QRectF(QPointF(0.0, 0.0), nodeSize);
}
