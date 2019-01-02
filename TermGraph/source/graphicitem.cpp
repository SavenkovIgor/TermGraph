#include "graphicitem.h"

GraphicItem::GraphicItem()
{
}

void GraphicItem::setParentItem(GraphicItem *item)
{
    parentItem = item;
}

void GraphicItem::setPos(QPointF pt)
{
    position = pt;
}

QPointF GraphicItem::pos() const
{
    return position;
}

QPointF GraphicItem::scenePos() const
{
    if (parentItem == nullptr) {
        return position;
    }

    return position + parentItem->scenePos();
}
