// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/model/graphicitem/rectgraphicitem.h"

RectGraphicItem::RectGraphicItem(const QPointF& pt, const QSizeF& size)
{
    setPos(pt);
    setSize(size);
}

QSizeF RectGraphicItem::getSize() const { return mSize; }

void RectGraphicItem::setSize(const QSizeF& size) { mSize = size; }

QRectF RectGraphicItem::getRect(CoordType coord) const
{
    switch (coord) {
    case CoordType::zeroPoint: return QRectF(QPointF(), mSize);
    case CoordType::local: return QRectF(pos(), mSize);
    case CoordType::scene: return QRectF(scenePos(), mSize);
    }

    assert(false); // must be unreachable
    return {};
}
