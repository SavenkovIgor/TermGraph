#pragma once

#include <QPointF>
#include <QRectF>

class GraphicItem
{
public:
    GraphicItem() = default;
    virtual ~GraphicItem() = default;

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
