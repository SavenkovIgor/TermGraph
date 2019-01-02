#ifndef GRAPHICITEM_H
#define GRAPHICITEM_H

#include <QPointF>
#include <QRectF>

class GraphicItem
{
public:
    GraphicItem();
    virtual ~GraphicItem() = 0;

    // Parents
    virtual void setParentItem(GraphicItem* item);

    // Position
    virtual void setPos(QPointF pt);
    virtual QPointF pos() const;
    virtual QPointF scenePos() const;

    // Size
    virtual QRectF boundingRect() const = 0;

private:
    QPointF position;

    GraphicItem* parentItem = nullptr;
};

#endif // GRAPHICITEM_H
