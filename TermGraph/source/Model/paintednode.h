#ifndef PAINTEDNODE_H
#define PAINTEDNODE_H

#include <QSqlRecord>
#include <QSizeF>

#include "graphterm.h"
#include "edge.h"

// For coordinate types
enum class CoordType {
    zeroPoint,  // Rect located in zero point
    local,      // Local coordinate system
    scene       // Scene coordinate system
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
    QRectF getFrameRect(CoordType inCoordinates) const;

    QPointF getCenter(CoordType inCoordinates) const;

    // Animation
    bool applyMove();
    void countForces();

    // Edges work
    int  getIntersections(bool swapped = false);
    qreal getSumEdgesLength(bool swap);

    void setSwap(QPointF toPt);
    void dropSwap();

protected:
    // --- Methods ---
    // Color tools
    QColor getBaseColor();
    QColor getSelectedColor();

    // Geometry tools
    // --- Methods ---
    void adjustRectSizeForName();

    // --- Variables ---
    // Hovers
    bool thisHovered = false;
    bool relativePaint = false;

private:
    // Scene tools
    static bool isNearPoints(QPointF pt1, QPointF pt2, qreal dist);

    // Neighbours tools
    PaintedNode* getNearestLeftNeigh();
    PaintedNode* getNearestRightNeigh();

    // Edges tools
    EdgesList getEdgesInLayer();

    // Rect tools
    static QRectF addMarginsToRect(QRectF rc, qreal mrg);

    // --- Variables ---
    static const qreal verScale;

    // Paint / Animation
    qreal newPosOffset = 0.0;

    QSizeF nodeSize = QSizeF(40.0, 10.0);
};

#endif // PAINTEDNODE_H
