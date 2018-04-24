#include "edge.h"

QColor Edge::getEdgeColor()
{
    switch (type) {
    case standart: return QColor(Qt::black);
    case termin: return QColor(Qt::blue);
    case description: return QColor(Qt::green);
    }
    return QColor(Qt::black);
}

Edge::Edge(TermNode *from, TermNode *to)
{
    this->toRoot = from;
    this->toBrnch   = to;

    from->addEdgeRef( this );
    to->addEdgeRef(   this );

    setAcceptHoverEvents(false);
    setZValue(0);
}

bool Edge::hasNode(TermNode *nd)
{
    if(nd == toRoot || nd == toBrnch)
        return true;

    return false;
}

bool Edge::isSameEdge(TermNode *rt, TermNode *br)
{
    bool rOk = (toRoot  == rt || toRoot  == br);
    bool bOk = (toBrnch == rt || toBrnch == br);

    return rOk && bOk;
}

bool Edge::isInGroupEdge()
{
    return toRoot->getGroupID() == toBrnch->getGroupID();
}

TermNode *Edge::getOtherSide(TermNode *n)
{
    if( n == toRoot )
        return toBrnch;

    else if( n == toBrnch )
        return toRoot;

    return nullptr;
}

QRectF Edge::boundingRect() const
{
    QPointF pt1 = toRoot->getCenter(),
            pt2 = toBrnch->getCenter();

    QRectF rc = QRectF(pt1,pt2);
    rc = rc.normalized();
    return rc;
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen p;
    int baseWidth = 2;

    if(toRoot->getGroupID() != toBrnch->getGroupID()) {
        p.setStyle(Qt::DashDotLine);
    } else {
        p.setStyle(Qt::SolidLine);
    }

    if( wide ) {
        baseWidth *= 2;
        p.setColor(QColor(0,166,147));
    } else {
        baseWidth /= 2;
    }

    p.setColor(getEdgeColor());

    QLineF edLine( toRoot->getCenter(), toBrnch->getCenter() );

    p.setWidth(baseWidth);
    painter->setPen(p);

    QLineF tmpLine;
    QPointF *ptTo = new QPointF();
    QPointF *ptFrom = new QPointF();

    for( Qt::Edge s : TermNode::sides ) {
        tmpLine = toBrnch->getRectLine( s );
        if( tmpLine.intersect( edLine, ptTo ) == QLineF::BoundedIntersection )
            break;
    }

    for( Qt::Edge s : TermNode::sides ) {
        tmpLine = toRoot->getRectLine( s );
        if( tmpLine.intersect( edLine, ptFrom ) == QLineF::BoundedIntersection )
            break;
    }

    if( ptTo->isNull() || ptFrom->isNull() ) {
        painter->drawLine( edLine );
        return;
    } else {
        painter->drawLine( QLineF( *ptFrom, *ptTo ) );
    }

    p.setStyle( Qt::SolidLine );
    painter->setPen(p);
    const qreal len     = -9;    // Negative len. Hack!)
    const qreal andOffs = 16;
    QLineF l1 = QLineF::fromPolar( len, edLine.angle() + andOffs );
    QLineF l2 = QLineF::fromPolar( len, edLine.angle() - andOffs );
    l1.translate( *ptTo );
    l2.translate( *ptTo );

    QPolygonF pol( QVector<QPointF>()<<*ptTo<<l1.p2()<<l2.p2() );

    painter->setBrush( QBrush( Qt::black ) );
    painter->drawConvexPolygon( pol );

    delete ptTo;
    delete ptFrom;
}

int Edge::getLayerDistance()
{
    return qAbs(toRoot->getPaintLevel() - toBrnch->getPaintLevel());
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

QLineF Edge::getLine( bool swap )
{
    QPointF pt1 = toRoot->getCenter(),
            pt2 = toBrnch->getCenter();

    if( swap ) {

        if( !swPtRoot.isNull() )
            pt1 = swPtRoot;

        if( !swPtBran.isNull() )
            pt2 = swPtBran;
    }

    return QLineF( pt1, pt2 );
}
