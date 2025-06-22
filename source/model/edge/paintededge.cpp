// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/model/edge/paintededge.h"

PaintedEdge::PaintedEdge(PaintedTerm::Ptr root, PaintedTerm::Ptr leaf)
    : Edge<PaintedTerm, EdgeData>(root, leaf, EdgeData())
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
    if (data().selectionType != resultSelection) {
        data().selectionType = resultSelection;
    }
}

void PaintedEdge::setSelectedBackward(bool value)
{
    auto resultSelection = value ? EdgeSelection::Type::Backward : EdgeSelection::Type::None;
    if (data().selectionType != resultSelection) {
        data().selectionType = resultSelection;
    }
}
