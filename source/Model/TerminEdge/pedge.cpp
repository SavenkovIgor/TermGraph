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

#include "pedge.h"

#include "source/Helpers/appstyle.h"

PEdge::PEdge(PaintedTerm::Ptr root, PaintedTerm::Ptr leaf)
    : Edge<PaintedTerm, EdgeData>(root, leaf, EdgeData())
    , GraphicItem()
{}

bool PEdge::isSelected() const
{
    return data().selectionType == EdgeSelection::forward || data().selectionType == EdgeSelection::backward;
}

bool PEdge::isHard() const { return data().type == EdgeType::terminHardLink; }

QPointF PEdge::rootPoint()
{
    auto paintedTerm = static_cast<PaintedTerm *>(root().get());
    return paintedTerm->getCenter(CoordType::scene);
}

QPointF PEdge::leafPoint()
{
    auto paintedTerm = static_cast<PaintedTerm *>(leaf().get());
    return paintedTerm->getCenter(CoordType::scene);
}

QColor PEdge::color() const
{
    switch (data().selectionType) {
    case EdgeSelection::backward: return AppStyle::Colors::Edges::selected;
    case EdgeSelection::forward: return AppStyle::Colors::Edges::selectedAlt;
    default: break;
    }

    switch (data().type) {
    case EdgeType::standart: return AppStyle::Colors::Edges::standard;
    case EdgeType::termin: return AppStyle::Colors::Edges::termin;
    case EdgeType::terminHardLink: return AppStyle::Colors::Edges::terminHardLink;
    case EdgeType::description: return AppStyle::Colors::Edges::description;
    case EdgeType::broken: return AppStyle::Colors::Edges::broken;
    case EdgeType::redundant: return AppStyle::Colors::Edges::standard;
    }

    return AppStyle::Colors::Edges::standard;
}

void PEdge::brokeEdge() { data().type = EdgeType::broken; }

void PEdge::setSelectedForward(bool value)
{
    auto resultSelection = value ? EdgeSelection::forward : EdgeSelection::none;
    if (data().selectionType != resultSelection)
        data().selectionType = resultSelection;
}

void PEdge::setSelectedBackward(bool value)
{
    auto resultSelection = value ? EdgeSelection::backward : EdgeSelection::none;
    if (data().selectionType != resultSelection)
        data().selectionType = resultSelection;
}
