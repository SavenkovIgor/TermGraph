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
    void setParentItem(GraphicItem* item);

    // Position
    void setPos(const QPointF& pt);
    void setPos(const qreal& x, const qreal& y);
    void moveBy(const QPointF& pt);
    void moveBy(const qreal& x, const qreal& y);
    QPointF pos() const;
    QPointF scenePos() const;

    // Z-value
    void setZValue(const int& z);

private:
    QPointF position;
    GraphicItem* parentItem = nullptr;
    int zValue = 0;
};

#endif // GRAPHICITEM_H
