#include "graphicitem.h"

GraphicItem::GraphicItem()  { }
GraphicItem::~GraphicItem() { }

void GraphicItem::setParentItem(GraphicItem *item)
{
    parentItem = item;
}

void GraphicItem::setParentItem(QGraphicsItem *item)
{
    grParentItem = item;
}

void GraphicItem::setPos(const QPointF &pt)
{
    position = pt;
}

void GraphicItem::setPos(const qreal& x, const qreal& y)
{
    position.setX(x);
    position.setY(y);
}

void GraphicItem::moveBy(const QPointF &pt)
{
    position += pt;
}

void GraphicItem::moveBy(const qreal &x, const qreal &y)
{
    position.rx() += x;
    position.ry() += y;
}

QPointF GraphicItem::pos() const
{
    return position;
}

QPointF GraphicItem::scenePos() const
{
//    if (parentItem == nullptr) {
//        return position;
//    }

//    return position + parentItem->scenePos();

    if (parentItem == nullptr && grParentItem == nullptr) {
        return position;
    }

    if (parentItem != nullptr) {
        return position + parentItem->scenePos();
    }

    if (grParentItem != nullptr) {
        return position + grParentItem->scenePos();
    }

    return position;
}

void GraphicItem::setZValue(const int &z)
{
    zValue = z;
}
