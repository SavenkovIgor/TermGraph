#ifndef EDGE_H
#define EDGE_H

#include <QPen>
#include <QPainter>
#include <QColor>
#include <QGraphicsItem>

#include "./graphedge.h"
#include "../Termin/paintedterm.h"
#include "../GraphicItem/graphicitem.h"
#include "../../Helpers/appstyle.h"

class PaintedTerm;

enum class EdgeType {
    standart,
    termin,
    description,
    broken
};

class Edge : public GraphEdge, public GraphicItem
{
public:
    Edge(PaintedTerm* toRoot, PaintedTerm* toLeaf, EdgeType type = EdgeType::termin);
    ~Edge() override;

    QRectF frameRect() const override;

    int getLayerDistance();

    qreal getXProjection();
    qreal getYProjection();

    QPointF swapPointRoot = QPointF();
    QPointF swapPointLeaf = QPointF();
    QLineF getLine(bool swap = false);

    static EdgesList castToEdgeList(GraphEdge::List lst);  // TODO: Delete!!!

    // Paint width
    void setSelected(bool value);

    QColor getEdgeColor() const;

    void brokeEdge();
    void cutOutFromSides();

private:
    bool selected = false;
    EdgeType type = EdgeType::termin;
};

#endif  // EDGE_H
