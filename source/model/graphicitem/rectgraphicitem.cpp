// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <QSizeF>

#include "source/model/coord_type.h"
#include "source/model/graphicitem/graphicitem.h"

export module RectGraphicItem;

export class RectGraphicItem : public GraphicItem
{
public:
    RectGraphicItem() = default;

    explicit RectGraphicItem(const QPointF& pt, const QSizeF& size)
    {
        setPos(pt);
        setSize(size);
    }

    ~RectGraphicItem() override = default;

    QSizeF getSize() const { return mSize; }

    void setSize(const QSizeF& size) { mSize = size; }

    QRectF getRect(CoordType coord) const
    {
        switch (coord) {
        case CoordType::zeroPoint: return QRectF(QPointF(), mSize);
        case CoordType::local: return QRectF(pos(), mSize);
        case CoordType::scene: return QRectF(scenePos(), mSize);
        }

        assert(false); // must be unreachable
        return {};
    }

private:
    QSizeF mSize;
};
