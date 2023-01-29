// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/model/graphicitem/graphicitem.h"

void GraphicItem::setParentItem(GraphicItem* parent) { mParent = parent; }

void GraphicItem::setPos(const QPointF& pt) { mPosition = pt; }

QPointF GraphicItem::pos() const { return mPosition; }

QPointF GraphicItem::scenePos() const
{
    if (mParent == nullptr) {
        return mPosition;
    }

    return mPosition + mParent->scenePos();
}
