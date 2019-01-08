#ifndef PAINTEDNODE_H
#define PAINTEDNODE_H

#include <QSqlRecord>
#include <QSizeF>

#include "./graphterm.h"
#include "../TerminEdge/edge.h"
#include "../enums.h"

// This class contains all info for paint Term somewhere
// Has functions about paint color, positioning. This kind of stuff
class PaintedTerm : public GraphTerm
{
public:
    bool needPaint = true;  // Flag for placing in paintQueue
    bool thisHovered = false;
    bool thisSelected = false;

    typedef QList<PaintedTerm*> List;

    static bool someoneHover;
    static bool someoneSelect;

    static QList<Qt::Edge> sides;

    PaintedTerm(QSqlRecord rec);
    ~PaintedTerm() override;

    // Leveling tools
    int getUpLevels(int pLevel = -1);

    // Painting tools
    void setRelatedPaint(bool val);
    QLineF getRectLine(Qt::Edge side);

    // Pure virtual functions
    virtual QPointF getPos() const = 0;
    virtual QPointF getScenePos() const = 0;

    virtual void movePosBy(qreal dx, qreal dy) = 0;

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

    // Color tools
    QColor getColor();

    qreal getCornerRadius();

    void setHover(const bool& hovered);
    void setSelection(const bool& selected);

protected:
    // Geometry tools
    // --- Methods ---
    void adjustRectSizeForName();

    // --- Variables ---
    // Hovers
    bool relativePaint = false;

    // Color tools
    QColor getBaseColor();
    QColor getSelectedColor();

private:
    // Scene tools
    static bool isNearPoints(QPointF pt1, QPointF pt2, qreal dist);

    // Neighbours tools
    PaintedTerm* getNearestLeftNeigh();
    PaintedTerm* getNearestRightNeigh();

    // Edges tools
    EdgesList getEdgesInLayer();

    // Rect tools
    static QRectF addMarginsToRect(QRectF rc, qreal mrg);

    // --- Variables ---
    static const qreal verScale;

    // Paint / Animation
    qreal newPosOffset = 0.0;

    QSizeF nodeSize = QSizeF(40.0, 10.0);

    qreal cornerRadius = 0;
    void updateCornerRadius();
};

#endif // PAINTEDNODE_H
