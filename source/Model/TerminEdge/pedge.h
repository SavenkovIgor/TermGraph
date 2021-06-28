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

#include "source/Model/Base/edge.hpp"
#include "source/Model/GraphicItem/graphicitem.h"
#include "source/Model/Termin/paintedterm.h"
#include "source/Model/TerminEdge/edgedata.h"

class PEdge : public Edge<PaintedTerm, EdgeData>, public GraphicItem
{
public:
    using Ptr  = std::shared_ptr<PEdge>;
    using List = std::vector<Ptr>;

    PEdge(PaintedTerm::Ptr root, PaintedTerm::Ptr leaf);

    void brokeEdge();
    void setSelectedForward(bool value);
    void setSelectedBackward(bool value);

    bool isSelected() const;
    bool isHard() const;

    QPointF rootPoint();
    QPointF leafPoint();

    QColor color() const;
};
