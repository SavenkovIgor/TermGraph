#ifndef GRAPHICITEM_H
#define GRAPHICITEM_H

#include <QPointF>
#include <QRectF>

class GraphicItem
{
public:
    GraphicItem();
    virtual ~GraphicItem();

    // Parents
    virtual void setParentItem(GraphicItem* item);

    // Position
    virtual void setPos(const QPointF& pt);
    virtual void setPos(const qreal& x, const qreal& y);
    virtual void moveBy(const QPointF& pt);
    virtual void moveBy(const qreal& x, const qreal& y);
    virtual QPointF pos() const;
    virtual QPointF scenePos() const;

    // Size
    virtual QRectF frameRect() const = 0;

    // Z-value
    void setZValue(const int& z);

private:
    QPointF position;
    GraphicItem* parentItem = nullptr;
    int zValue = 0;
};

#endif // GRAPHICITEM_H
