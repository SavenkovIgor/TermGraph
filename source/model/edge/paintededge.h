// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <memory>
#include <vector>

#include "source/Graph/Edge.hpp"

#include "source/model/edge/edgedata.h"
#include "source/model/graphicitem/graphicitem.h"
#include "source/model/term/paintedterm.h"

class PaintedEdge : public graph::Edge<PaintedTerm, EdgeData>, public GraphicItem
{
public:
    using Ptr  = std::shared_ptr<PaintedEdge>;
    using List = std::vector<Ptr>;

    static inline auto asListSize = [](auto num) -> List::size_type { return static_cast<List::size_type>(num); };

    PaintedEdge(PaintedTerm::Ptr root, PaintedTerm::Ptr leaf);

    void brokeEdge();
    void setSelectedForward(bool value);
    void setSelectedBackward(bool value);

    bool isSelected() const;
    bool isHard() const;

    QPointF rootPoint();
    QPointF leafPoint();
};
