#include "rectgraphicitem.h"

RectGraphicItem::RectGraphicItem() : GraphicItem () { }

RectGraphicItem::RectGraphicItem(const QPointF &pt, const QSizeF &size)
{
    setPos(pt);
    setSize(size);
}

RectGraphicItem::~RectGraphicItem() { }

QRectF RectGraphicItem::boundingRect() const
{
//    return getRect();
    return QRectF();
}

void RectGraphicItem::setSize(const QSizeF& size)
{
    _size = size;
}

QRectF RectGraphicItem::getRect() const
{
    // make pos - center point
    auto pt = pos();
    pt.rx() -= _size.width()/2;
    pt.ry() -= _size.height()/2;
    return QRectF(pt, _size);
}
