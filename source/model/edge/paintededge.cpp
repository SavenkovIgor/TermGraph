// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <memory>
#include <vector>

#include <QPointF>

export module PaintedEdge;

import EdgeData;
import Enums.CoordType;
import Enums.EdgeSelection;
import Enums.EdgeType;
import Graph;
import GraphicItem;
import PaintedTerm;

using namespace std;

export class PaintedEdge : public graph::Edge<PaintedTerm, EdgeData>, public GraphicItem
{
public:
    using Ptr  = shared_ptr<PaintedEdge>;
    using List = vector<Ptr>;

    static inline auto asListSize = [](auto num) -> List::size_type { return static_cast<List::size_type>(num); };

    PaintedEdge(PaintedTerm::Ptr root, PaintedTerm::Ptr leaf)
        : Edge<PaintedTerm, EdgeData>(root, leaf, EdgeData())
    {}

    void brokeEdge() { data().type = EdgeType::Broken; }

    void setSelectedForward(bool value)
    {
        auto resultSelection = value ? EdgeSelection::Forward : EdgeSelection::None;
        if (data().selectionType != resultSelection) {
            data().selectionType = resultSelection;
        }
    }

    void setSelectedBackward(bool value)
    {
        auto resultSelection = value ? EdgeSelection::Backward : EdgeSelection::None;
        if (data().selectionType != resultSelection) {
            data().selectionType = resultSelection;
        }
    }

    bool isSelected() const
    {
        return data().selectionType == EdgeSelection::Forward || data().selectionType == EdgeSelection::Backward;
    }

    bool isHard() const { return data().type == EdgeType::TerminHardLink; }

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
