/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
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

#include "source/Model/GraphicItem/graphicitem.h"

void GraphicItem::setParentItem(GraphicItem* parent) { mParent = parent; }

void GraphicItem::setPos(const QPointF& pt) { mPosition = pt; }

QPointF GraphicItem::pos() const { return mPosition; }

QPointF GraphicItem::scenePos() const
{
    if (mParent == nullptr)
        return mPosition;

    return mPosition + mParent->scenePos();
}
