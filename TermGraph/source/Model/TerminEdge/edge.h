#ifndef EDGE_H
#define EDGE_H

#include <QPen>
#include <QPainter>
#include <QColor>
#include <QGraphicsItem>

#include "./graphedge.h"
#include "../Termin/paintedterm.h"
#include "../../Helpers/appstyle.h"

class PaintedTerm;

enum class EdgeType {
    standart,
    termin,
    description,
    broken
};

class Edge : public GraphEdge, public QGraphicsItem
{
public:
    Edge(PaintedTerm* toRoot, PaintedTerm* toLeaf, EdgeType type = EdgeType::termin);

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

    // Paint width
    bool getWide() const;
    void setWide(bool value);

    // Scene stuff
    void setSceneParent(QGraphicsItem* item);

    QColor getEdgeColor() const;

    void brokeEdge();
    void cutOutFromSides();

private:
    bool wide = false;
    EdgeType type = EdgeType::termin;
};

#endif  // EDGE_H
