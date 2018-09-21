#ifndef PAINTEDNODE_H
#define PAINTEDNODE_H

#include <QSqlRecord>
#include <QSizeF>

#include "graphterm.h"
#include "edge.h"

// For coordinate types
enum class CoordType {
    none,   // Default
    local,  // Local coordinate system
    scene   // Scene coordinate system
};

class PaintedNode : public GraphTerm
{
public:
    static bool someoneHover;
    static bool someoneSelect;

    static QList<Qt::Edge> sides;

    PaintedNode(QSqlRecord rec);

    // Leveling tools
    int getUpLevels(int pLevel = -1);

    // Painting tools

    void setRelatedPaint(bool val);
    QLineF getRectLine(Qt::Edge side);

    // Pure virtual functions
    virtual QPointF getPos() const = 0;
    virtual QPointF getScenePos() const = 0;

    virtual void movePosBy(qreal dx, qreal dy) = 0;
    virtual void PrepareGeometryChangeCall() = 0;

    // NodeRect tools
    QRectF getNodeRect(CoordType inCoordinates) const;
    QRectF getFrameRect(CoordType inCoordinates) const;  // TODO: Realize!

    QPointF getCenter(CoordType inCoordinates) const;

    QRectF getRcWithBorders() const;
    QSizeF getSize(bool withBorder = true) const;

    // Animation
    bool applyMove();
    void countForces();

    // Edges work
    int  getIntersections(bool swapped = false);
    EdgesList getEdgesInLayer();
    void dropSwap();
    void setSwap(QPointF toPt);
    qreal getSumEdgesLength(bool swap);

    void adjustSizeForName();

protected:
    // Color tools
    QColor getBaseColor();
    QColor getSelectedColor();

    // Neighbours tools
    PaintedNode* getNearestLeftNeigh();
    PaintedNode* getNearestRightNeigh();

//    EdgesList getEdgesInLayer();

    // Scene tools
    static bool isNearPoints(QPointF pt1, QPointF pt2, qreal dist);

    // Geometry tools
    QRectF getInnerNodeRect() const;

    // Hovers
    bool thisHovered = false;
    bool thisSelected = false;
    bool relativePaint = false;

    QSizeF nodeSize = QSizeF(40.0, 10.0);

    // Paint / Animation
    qreal newPosOffs = 0.0;
private:
    static const qreal verScale;
};

#endif // PAINTEDNODE_H
