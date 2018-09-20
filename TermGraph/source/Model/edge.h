#ifndef EDGE_H
#define EDGE_H

#include <QPen>
#include <QPainter>
#include <QColor>
#include <QGraphicsItem>

#include "./graphedge.h"
#include "./termnode.h"
#include "../Helpers/appstyle.h"

enum EdgeType{
    standart,
    termin,
    description
};

class Edge : public GraphEdge, public QGraphicsItem
{
//    Q_OBJECT

private:

    EdgeType type = termin;

    QColor getEdgeColor();

public:
    bool wide = false;

public:
    Edge(TermNode *toRoot, TermNode *toLeaf, EdgeType type = EdgeType::termin);

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*);

    int getLayerDistance();

    qreal getXProjection();
    qreal getYProjection();

//    QPointF swP1 = QPointF();
    QPointF swapPointRoot = QPointF();
    QPointF swapPointLeaf = QPointF();
    QLineF getLine(bool swap = false);

    static EdgesList castToEdgeList(GraphEdge::List lst);  // TODO: Delete!!!
};

#endif  // EDGE_H
