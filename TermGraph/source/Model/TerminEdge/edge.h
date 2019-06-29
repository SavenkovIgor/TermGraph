#ifndef EDGE_H
#define EDGE_H

#include <QColor>

#include "source/Model/TerminEdge/graphedge.h"
#include "source/Model/Termin/paintedterm.h"
#include "source/Model/GraphicItem/graphicitem.h"
#include "source/Helpers/appstyle.h"

class PaintedTerm;

enum class EdgeType {
    standart,
    termin,
    description,
    broken
};

enum class EdgeSelected {
    none,
    forward,
    backward
};

class Edge : public GraphEdge, public GraphicItem
{
public:
    Edge(PaintedTerm* toRoot, PaintedTerm* toLeaf, EdgeType type = EdgeType::termin);
    ~Edge() override = default;

    QRectF edgeRect() const;

    int getLayerDistance();

    qreal getXProjection();
    qreal getYProjection();

    QPointF swapPointRoot = QPointF();
    QPointF swapPointLeaf = QPointF();
    QLineF getLine(bool swap = false);

    static EdgesList castToEdgeList(GraphEdge::List lst);  // TODO: Delete!!!

    // Paint width
    void setSelectedForward(bool value);
    void setSelectedBackward(bool value);

    QColor getEdgeColor() const;

    void brokeEdge();
    void cutOutFromSides();

private:
    EdgeSelected selected = EdgeSelected::none;
    EdgeType type = EdgeType::termin;
};

#endif  // EDGE_H
