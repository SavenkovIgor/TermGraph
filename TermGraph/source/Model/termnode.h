#ifndef TERMNODE_H
#define TERMNODE_H

#include <QGraphicsItem>
#include <QDebug>

#include "../Helpers/handytypes.h"
#include "./edge.h"
#include "../Helpers/appstyle.h"
#include "../Helpers/helpstuff.h"
#include "./paintednode.h"

enum class CoordType {
    none,
    local,
    scene
};

class TermNode: public PaintedNode, public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    TermNode(QSqlRecord rec);
    ~TermNode();

    static QList< Qt::Edge > sides;

    static const qreal verScale;

    bool relative = false;
    void setRelatPaint(bool val);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *evt);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *evt);

    QRectF getNodeRect(CoordType inCoordinates) const;
    QRectF getFrameRect(CoordType inCoordinates) const;

    QPointF getCenter(CoordType inCoordinates) const;

    QRectF boundingRect() const;
    QRectF getRcWithBorders();
    QSizeF getSize(bool withBorder = true);

    QLineF getRectLine(Qt::Edge side);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget *);

    void mousePressEvent(QGraphicsSceneMouseEvent *evt);

    void countForces();
    int  getIntersections(bool swapped  = false);
    qreal getSumEdgesLength(bool swap);
    void setSwap(QPointF toPt);
    void dropSwap();

    bool applyMove();

    QString getDebugString();

private:
    EdgesList getEdgesInLayer();

    // Initialize
    void adjustSizeForName();

    // Positioning
    TermNode* getNearestLeftNeigh();
    TermNode* getNearestRightNeigh();
};

#endif  // TERMNODE_H
