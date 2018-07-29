#include "edge.h"

QColor Edge::getEdgeColor()
{
    switch (type) {
    case standart: return AppStyle::Colors::Edges::standard;
    case termin: return AppStyle::Colors::Edges::termin;
    case description: return AppStyle::Colors::Edges::description;
    }
    return QColor(Qt::black);
}

Edge::Edge(TermNode *from, TermNode *to, EdgeType type)
{
    this->toRoot = from;
    this->toLeaf = to;
    this->type = type;

    from->addEdgeRef(this);
    to->addEdgeRef(this);

    setAcceptHoverEvents(false);
    setZValue(0);
}

bool Edge::hasNode(TermNode *node)
{
    if (node == toRoot || node == toLeaf)
        return true;

    return false;
}

bool Edge::isSameEdge(TermNode *rt, TermNode *br)
{
    bool rOk = (toRoot == rt || toRoot == br);
    bool bOk = (toLeaf == rt || toLeaf == br);

    return rOk && bOk;
}

bool Edge::isInGroupEdge()
{
    return toRoot->getGroupUuid() == toLeaf->getGroupUuid();
}

TermNode *Edge::getOtherSide(TermNode *n)
{
    if (n == toRoot) {
        return toLeaf;
    } else if (n == toLeaf) {
        return toRoot;
    }

    return nullptr;
}

QRectF Edge::boundingRect() const
{
    QPointF pt1 = toRoot->getCenter(CoordType::scene),
            pt2 = toLeaf->getCenter(CoordType::scene);

    QRectF rc = QRectF(pt1, pt2);
    rc = rc.normalized();
    return rc;
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen p;
    p.setStyle(isInGroupEdge() ? Qt::SolidLine : Qt::DashDotLine);
    p.setColor(getEdgeColor());

    int baseWidth = 3;
    if (wide) {
        baseWidth *= 2;
        p.setColor(AppStyle::Colors::Edges::selected);
    } else {
        baseWidth /= 2;
    }

    QLineF edLine(toRoot->getCenter(CoordType::local), toLeaf->getCenter(CoordType::local));

    p.setWidth(baseWidth);
    painter->setPen(p);

    QLineF tmpLine;
    QPointF *ptTo = new QPointF();
    QPointF *ptFrom = new QPointF();

    for (Qt::Edge side : TermNode::sides) {
        tmpLine = toLeaf->getRectLine(side);
        if (tmpLine.intersect(edLine, ptTo) == QLineF::BoundedIntersection) {
            break;
        }
    }

    for (Qt::Edge side : TermNode::sides) {
        tmpLine = toRoot->getRectLine(side);
        if (tmpLine.intersect(edLine, ptFrom) == QLineF::BoundedIntersection) {
            break;
        }
    }

    if (ptTo->isNull() || ptFrom->isNull()) {
        painter->drawLine(edLine);
        return;
    } else {
        painter->drawLine(QLineF(*ptFrom, *ptTo));
    }

    p.setStyle(Qt::SolidLine);
    painter->setPen(p);
    const qreal len     = -9;    // Negative len. Hack!)
    const qreal andOffs = 16;
    QLineF l1 = QLineF::fromPolar(len, edLine.angle() + andOffs);
    QLineF l2 = QLineF::fromPolar(len, edLine.angle() - andOffs);
    l1.translate(*ptTo);
    l2.translate(*ptTo);

    QPolygonF pol(QVector<QPointF>() << *ptTo << l1.p2() << l2.p2());

    painter->setBrush(getEdgeColor());
    painter->drawConvexPolygon(pol);

    delete ptTo;
    delete ptFrom;
}

int Edge::getLayerDistance()
{
    return qAbs(toRoot->getPaintLevel() - toLeaf->getPaintLevel());
}

qreal Edge::getXProjection()
{
//    if( toRoot->getPaintLevel() == -1 || toBrnch->getPaintLevel() == -1 )
//        return 0.0;

    return boundingRect().width();
}

qreal Edge::getYProjection()
{
    return boundingRect().height();
}

QLineF Edge::getLine(bool swap)
{
    QPointF pt1 = toRoot->getCenter(CoordType::scene),
            pt2 = toLeaf->getCenter(CoordType::scene);

    if (swap) {
        if (!swPtRoot.isNull())
            pt1 = swPtRoot;

        if (!swPtBran.isNull())
            pt2 = swPtBran;
    }

    return QLineF( pt1, pt2 );
}
