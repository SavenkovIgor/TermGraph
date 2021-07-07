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

#include "source/Model/Base/forest.hpp"
#include "source/Model/GraphicItem/rectgraphicitem.h"
#include "source/Model/Termin/paintedterm.h"
#include "source/Model/TerminEdge/paintededge.h"
#include "source/Model/TerminGroup/nodeverticalstack.h"

class NodeVerticalStack;

class PaintedForest : public Forest<PaintedTerm, PaintedEdge>
{
public:
    using Ptr  = std::shared_ptr<PaintedForest>;
    using List = std::vector<Ptr>;

    // Constructor/Destructor
    PaintedForest(const GraphData<PaintedTerm, PaintedEdge>& data);
    ~PaintedForest() = default;

    RectGraphicItem& rect();

    // TreeInfo
    bool hasTerm(PaintedTerm::Ptr term) const;

    QRectF getTreeRect(CoordType inCoordinates) const;
    QSizeF baseSize() const; // Clearly counted value. Ignoring real node positions
    double square() const;

    // Internal counts and preparations
    void setTreeNodeCoords(QPointF leftTopPoint = QPointF());

    opt<QPointF> optimalRootsBasedPosition(const PaintedTerm::Ptr term) const;

    PaintedTerm::OptPtr getNodeAtPoint(const QPointF& pt) const;

    QString getHierarchyDefinition(PaintedTerm::Ptr term);

    void selectTerm(const PaintedTerm::Ptr& term, bool selected);

    // Deletions
    PaintedForest(const PaintedForest&) = delete;

private:
    // Internal Info
    PaintedTerm::List getAllNodesInTree() const;
    qreal             getMaxStackHeight() const;

private: // Members
    std::vector<NodeVerticalStack> mStacks;
    RectGraphicItem                mRect;
};
