#ifndef GRAPHICITEM_H
#define GRAPHICITEM_H

#include <QPointF>
#include <QRectF>
#include <QGraphicsItem>

class GraphicItem
{
public:
    GraphicItem();
    virtual ~GraphicItem();

    // Parents
    virtual void setParentItem(GraphicItem* item);
    virtual void setParentItem(QGraphicsItem* item);  // TODO: Delete later

    // Position
    virtual void setPos(const QPointF& pt);
    virtual void setPos(const qreal& x, const qreal& y);
    virtual void moveBy(const QPointF& pt);
    virtual void moveBy(const qreal& x, const qreal& y);
    virtual QPointF pos() const;
    virtual QPointF scenePos() const;

    // Size
    virtual QRectF boundingRect() const = 0;

    // Z-value
    void setZValue(const int& z);

private:
    QPointF position;
    GraphicItem* parentItem = nullptr;
    QGraphicsItem* grParentItem = nullptr;  // TODO: Delete later
    int zValue = 0;
};

#endif // GRAPHICITEM_H
