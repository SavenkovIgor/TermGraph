#ifndef TERMNODE_H
#define TERMNODE_H

#include <QGraphicsItem>
#include <QDebug>

#include "../Helpers/handytypes.h"
#include "./edge.h"
#include "../Helpers/appstyle.h"
#include "../Helpers/helpstuff.h"
#include "./paintednode.h"

class TermNode: public PaintedNode, public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    TermNode(QSqlRecord rec);
    ~TermNode();

    static QList< Qt::Edge > sides;
    static const qreal verScale;

    void setRelatedPaint(bool val);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *evt);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *evt);

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget *);

    void mousePressEvent(QGraphicsSceneMouseEvent *evt);

    void countForces();
    int  getIntersections(bool swapped  = false);
    qreal getSumEdgesLength(bool swap);
    void setSwap(QPointF toPt);
    void dropSwap();

    bool applyMove();

    QString getDebugString();

    // Overriding of PaintedNode
    QPointF getPos() const override;
    QPointF getScenePos() const override;

private:
    EdgesList getEdgesInLayer();

    // Initialize
    void adjustSizeForName();

    // Positioning
    TermNode* getNearestLeftNeigh();
    TermNode* getNearestRightNeigh();
};

#endif  // TERMNODE_H
