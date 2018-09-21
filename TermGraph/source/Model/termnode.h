#ifndef TERMNODE_H
#define TERMNODE_H

#include <QGraphicsItem>
#include <QDebug>

//#include "../Helpers/handytypes.h"
#include "../Helpers/appstyle.h"
#include "../Helpers/helpstuff.h"
#include "./paintednode.h"

class TermNode: public PaintedNode, public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    TermNode(QSqlRecord rec);

    // Overriding of QGraphicsItem
    void mousePressEvent(QGraphicsSceneMouseEvent *evt) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *evt) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *evt) override;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget *) override;

    // Overriding of PaintedNode
    QPointF getPos() const override;
    QPointF getScenePos() const override;
    void movePosBy(qreal dx, qreal dy) override;
    void PrepareGeometryChangeCall() override;

private:
    QString getDebugString();
};

#endif  // TERMNODE_H
