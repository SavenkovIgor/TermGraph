#include "rectgraphicitem.h"

RectGraphicItem::RectGraphicItem() : GraphicItem () { }

RectGraphicItem::RectGraphicItem(const QPointF &pt, const QSizeF &size)
{
    setPos(pt);
    setSize(size);
}

RectGraphicItem::~RectGraphicItem() { }

QRectF RectGraphicItem::frameRect() const
{
    return getRect();
}

void RectGraphicItem::setSize(const QSizeF& size)
{
    _size = size;
}

QSizeF RectGraphicItem::size() const
{
    return _size;
}

QRectF RectGraphicItem::getRect() const  // TODO: Write full function with parameter
{
    return QRectF(pos(), _size);
}
