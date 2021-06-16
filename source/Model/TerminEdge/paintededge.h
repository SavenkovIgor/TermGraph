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

#include <vector>

#include <QColor>
#include <QLineF>
#include <QPointF>

#include "source/Model/GraphicItem/graphicitem.h"
#include "source/Model/TerminEdge/edgedata.h"
#include "source/Model/TerminEdge/graphedge.h"
#include "source/Model/enums.h"

class PaintedTerm;

class PaintedEdge : public GraphEdge, public GraphicItem
{
public:
    using Ptr        = std::shared_ptr<PaintedEdge>;
    using UnsafeList = std::vector<PaintedEdge*>;
    using List       = std::vector<Ptr>;

    explicit PaintedEdge(PaintedTerm* toRoot, PaintedTerm* toLeaf, EdgeType type = EdgeType::termin);
    ~PaintedEdge() override = default;

    QRectF edgeRect() const;

    QPointF rootPoint() const;
    QPointF leafPoint() const;
    QColor  getEdgeColor() const;

    void brokeEdge();
    void makeEdgeRedundant();
    void cutOutFromSides();

    // Selection
    void setSelectedForward(bool value);
    void setSelectedBackward(bool value);

    EdgeSelection selectedType() const;
    bool          isSelected() const;
    bool          isBroken() const;
    bool          isRedundant() const;
    bool          isHard() const;

private:
    EdgeData mData;
};
