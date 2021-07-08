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

#pragma once

#include <memory>
#include <vector>

#include "source/model/base/edge.hpp"
#include "source/model/edge/edgedata.h"
#include "source/model/graphicitem/graphicitem.h"
#include "source/model/term/paintedterm.h"

class PaintedEdge : public Edge<PaintedTerm, EdgeData>, public GraphicItem
{
public:
    using Ptr  = std::shared_ptr<PaintedEdge>;
    using List = std::vector<Ptr>;

    PaintedEdge(PaintedTerm::Ptr root, PaintedTerm::Ptr leaf);

    void brokeEdge();
    void setSelectedForward(bool value);
    void setSelectedBackward(bool value);

    bool isSelected() const;
    bool isHard() const;

    QPointF rootPoint();
    QPointF leafPoint();

    QColor color() const; // TODO: Move to model or qml
};
