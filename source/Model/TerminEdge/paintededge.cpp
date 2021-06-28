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

#include "source/Model/TerminEdge/paintededge.h"

#include "source/Helpers/appstyle.h"
#include "source/Model/Termin/paintedterm.h"

PaintedEdge::PaintedEdge(TermPtr toRoot, TermPtr toLeaf, EdgeType type)
    : GraphEdge(toRoot, toLeaf)
    , GraphicItem()
{
    mData.type = type;
}

QColor PaintedEdge::getEdgeColor() const
{
    switch (mData.selectionType) {
    case EdgeSelection::backward: return AppStyle::Colors::Edges::selected;
    case EdgeSelection::forward: return AppStyle::Colors::Edges::selectedAlt;
    default: break;
    }

    switch (mData.type) {
    case EdgeType::standart: return AppStyle::Colors::Edges::standard;
    case EdgeType::termin: return AppStyle::Colors::Edges::termin;
    case EdgeType::terminHardLink: return AppStyle::Colors::Edges::terminHardLink;
    case EdgeType::description: return AppStyle::Colors::Edges::description;
    case EdgeType::broken: return AppStyle::Colors::Edges::broken;
    case EdgeType::redundant: return AppStyle::Colors::Edges::standard;
    }

    return AppStyle::Colors::Edges::standard;
}

EdgeSelection PaintedEdge::selectedType() const { return mData.selectionType; }

bool PaintedEdge::isSelected() const
{
    return mData.selectionType == EdgeSelection::forward || mData.selectionType == EdgeSelection::backward;
}

bool PaintedEdge::isBroken() const { return mData.type == EdgeType::broken; }

bool PaintedEdge::isRedundant() const { return mData.type == EdgeType::redundant; }

bool PaintedEdge::isHard() const { return mData.type == EdgeType::terminHardLink; }

QRectF PaintedEdge::edgeRect()
{
    QPointF pt1 = rootPoint();
    QPointF pt2 = leafPoint();

    QRectF rc = QRectF(pt1, pt2);
    rc        = rc.normalized();
    return rc;
}

QPointF PaintedEdge::rootPoint()
{
    auto paintedTerm = static_cast<PaintedTerm *>(root().get());
    return paintedTerm->getCenter(CoordType::scene);
}

QPointF PaintedEdge::leafPoint()
{
    auto paintedTerm = static_cast<PaintedTerm *>(leaf().get());
    return paintedTerm->getCenter(CoordType::scene);
}
