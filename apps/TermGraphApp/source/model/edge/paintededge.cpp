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

#include "source/model/edge/paintededge.h"

#include "source/helpers/appstyle.h"

PaintedEdge::PaintedEdge(PaintedTerm::Ptr root, PaintedTerm::Ptr leaf)
    : Edge<PaintedTerm, EdgeData>(root, leaf, EdgeData())
    , GraphicItem()
{}

bool PaintedEdge::isSelected() const
{
    return data().selectionType == EdgeSelection::Type::Forward
        || data().selectionType == EdgeSelection::Type::Backward;
}

bool PaintedEdge::isHard() const { return data().type == EdgeType::Type::TerminHardLink; }

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

void PaintedEdge::brokeEdge() { data().type = EdgeType::Type::Broken; }

void PaintedEdge::setSelectedForward(bool value)
{
    auto resultSelection = value ? EdgeSelection::Type::Forward : EdgeSelection::Type::None;
    if (data().selectionType != resultSelection)
        data().selectionType = resultSelection;
}

void PaintedEdge::setSelectedBackward(bool value)
{
    auto resultSelection = value ? EdgeSelection::Type::Backward : EdgeSelection::Type::None;
    if (data().selectionType != resultSelection)
        data().selectionType = resultSelection;
}
