#include "rectgraphicitem.h"

RectGraphicItem::RectGraphicItem() : GraphicItem () { }

RectGraphicItem::RectGraphicItem(const QPointF &pt, const QSizeF &size)
{
    setPos(pt);
    setSize(size);
}

RectGraphicItem::~RectGraphicItem() { }

void RectGraphicItem::setSize(const QSizeF& size)
{
    _size = size;
}

QRectF RectGraphicItem::getRect(CoordType coord) const
{
    switch (coord) {
    case CoordType::zeroPoint: return QRectF(QPointF(),  _size);
    case CoordType::local:     return QRectF(pos(),      _size);
    case CoordType::scene:     return QRectF(scenePos(), _size);
    }
}

QSizeF RectGraphicItem::getSize() const
{
    return _size;
}
