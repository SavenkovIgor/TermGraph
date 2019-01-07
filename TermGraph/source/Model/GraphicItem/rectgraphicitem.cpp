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

QRectF RectGraphicItem::getRect() const
{
    // make pos - center point
    auto pt = pos();
    pt.rx() -= _size.width()/2;
    pt.ry() -= _size.height()/2;
    return QRectF(pt, _size);
}
