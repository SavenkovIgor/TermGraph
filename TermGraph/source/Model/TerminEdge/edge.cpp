#include "edge.h"

QColor Edge::getEdgeColor() const
{
    if (selected) {
        return AppStyle::Colors::Edges::selected;
    }

    switch (type) {
    case EdgeType::standart: return AppStyle::Colors::Edges::standard;
    case EdgeType::termin: return AppStyle::Colors::Edges::termin;
    case EdgeType::description: return AppStyle::Colors::Edges::description;
    case EdgeType::broken: return AppStyle::Colors::Edges::broken;
    }

    return AppStyle::Colors::Edges::standard;
}

void Edge::brokeEdge()
{
    getRoot()->removeEdgeToLeafs(this);
    getRoot()->addBrokenEdge(this);
    getLeaf()->removeEdgeToRoots(this);
    type = EdgeType::broken;
}

void Edge::cutOutFromSides()
{
    getRoot()->removeEdgeToLeafs(this);
    getLeaf()->removeEdgeToRoots(this);
}

Edge::Edge(PaintedTerm* from, PaintedTerm* to, EdgeType type) :
    GraphEdge(from, to)
{
    this->type = type;
}

QRectF Edge::edgeRect() const
{
    QPointF pt1 = dynamic_cast<PaintedTerm*>(getRoot())->getCenter(CoordType::scene);
    QPointF pt2 = dynamic_cast<PaintedTerm*>(getLeaf())->getCenter(CoordType::scene);

    QRectF rc = QRectF(pt1, pt2);
    rc = rc.normalized();
    return rc;
}

int Edge::getLayerDistance()
{
    auto toRoot = dynamic_cast<PaintedTerm*>(getRoot());
    auto toLeaf = dynamic_cast<PaintedTerm*>(getLeaf());

    return qAbs(toRoot->getPaintLevel() - toLeaf->getPaintLevel());
}

qreal Edge::getXProjection()
{
    return edgeRect().width();
}

qreal Edge::getYProjection()
{
    return edgeRect().height();
}

QLineF Edge::getLine(bool swap)
{
    auto toRoot = dynamic_cast<PaintedTerm*>(getRoot());
    auto toLeaf = dynamic_cast<PaintedTerm*>(getLeaf());

    QPointF pt1 = toRoot->getCenter(CoordType::scene);
    QPointF pt2 = toLeaf->getCenter(CoordType::scene);

    if (swap) {
        if (!swapPointRoot.isNull())
            pt1 = swapPointRoot;

        if (!swapPointLeaf.isNull())
            pt2 = swapPointLeaf;
    }

    return QLineF( pt1, pt2 );
}

EdgesList Edge::castToEdgeList(GraphEdge::List lst)
{
    EdgesList edgeLst;
    for (auto graphEdg : lst) {
        edgeLst << dynamic_cast<Edge*>(graphEdg);
    }
    return  edgeLst;
}

void Edge::setSelected(bool value)
{
    selected = value;
    needPaint = true;
}
