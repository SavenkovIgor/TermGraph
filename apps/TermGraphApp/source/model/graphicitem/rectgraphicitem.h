// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QSizeF>

#include "source/model/enums.h"
#include "source/model/graphicitem/graphicitem.h"

class RectGraphicItem : public GraphicItem
{
public:
    // Constructors
    RectGraphicItem() = default;
    explicit RectGraphicItem(const QPointF& pt, const QSizeF& getSize);

    // Destructor
    ~RectGraphicItem() override = default;

    QSizeF getSize() const;
    void   setSize(const QSizeF& getSize);

    QRectF getRect(CoordType coord) const;

private:
    QSizeF mSize;
};
