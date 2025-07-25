// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <memory>
#include <vector>

#include "source/model/edge/edgedata.h"
#include "source/model/graphicitem/graphicitem.h"

export module PaintedEdge;

import Graph.Edge;
import PaintedTerm;

export class PaintedEdge : public graph::Edge<PaintedTerm, EdgeData>, public GraphicItem
{
public:
    using Ptr  = std::shared_ptr<PaintedEdge>;
    using List = std::vector<Ptr>;

    static inline auto asListSize = [](auto num) -> List::size_type { return static_cast<List::size_type>(num); };

    PaintedEdge(PaintedTerm::Ptr root, PaintedTerm::Ptr leaf)
        : Edge<PaintedTerm, EdgeData>(root, leaf, EdgeData())
    {}

    void brokeEdge() { data().type = EdgeType::Type::Broken; }

    void setSelectedForward(bool value)
    {
        auto resultSelection = value ? EdgeSelection::Type::Forward : EdgeSelection::Type::None;
        if (data().selectionType != resultSelection) {
            data().selectionType = resultSelection;
        }
    }

    void setSelectedBackward(bool value)
    {
        auto resultSelection = value ? EdgeSelection::Type::Backward : EdgeSelection::Type::None;
        if (data().selectionType != resultSelection) {
            data().selectionType = resultSelection;
        }
    }

    bool isSelected() const
    {
        return data().selectionType == EdgeSelection::Type::Forward
            || data().selectionType == EdgeSelection::Type::Backward;
    }

    bool isHard() const { return data().type == EdgeType::Type::TerminHardLink; }

    QPointF rootPoint()
    {
        auto paintedTerm = static_cast<PaintedTerm *>(root().get());
        return paintedTerm->getCenter(CoordType::scene);
    }

    QPointF leafPoint()
    {
        auto paintedTerm = static_cast<PaintedTerm *>(leaf().get());
        return paintedTerm->getCenter(CoordType::scene);
    }
};
