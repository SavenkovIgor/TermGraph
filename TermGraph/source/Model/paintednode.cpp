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
    EdgesGraphList ret;
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
