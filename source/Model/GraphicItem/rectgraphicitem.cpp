/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

#include "source/Model/GraphicItem/rectgraphicitem.h"

RectGraphicItem::RectGraphicItem()
    : GraphicItem()
{}

RectGraphicItem::RectGraphicItem(const QPointF& pt, const QSizeF& size)
{
    setPos(pt);
    setSize(size);
}

RectGraphicItem::~RectGraphicItem() {}

void RectGraphicItem::setSize(const QSizeF& size)
{
    _size = size;
}

QRectF RectGraphicItem::getRect(CoordType coord) const
{
    switch (coord) {
    case CoordType::zeroPoint:
        return QRectF(QPointF(), _size);
    case CoordType::local:
        return QRectF(pos(), _size);
    case CoordType::scene:
        return QRectF(scenePos(), _size);
    }
}

QSizeF RectGraphicItem::getSize() const
{
    return _size;
}
