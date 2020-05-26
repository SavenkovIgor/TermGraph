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

#include <stdlib.h>
#include <QList>

#include "source/Model/GraphicItem/rectgraphicitem.h"
#include "source/Model/Termin/paintedterm.h"
#include "source/Model/TerminEdge/edge.h"
#include "source/Model/TerminGroup/nodeverticalstack.h"

class TermTree
{
public:
    using List = QList<TermTree*>;

    // Constructor/Destructor
    TermTree();
    ~TermTree();

    RectGraphicItem* rect = nullptr;

    // Add info
    void addTerm(PaintedTerm* term);

    // TreeInfo
    bool hasTerm(PaintedTerm* term) const;
    bool hasEdge(Edge* edge) const;

    QRectF getTreeRect(CoordType inCoordinates) const;
    QSizeF getTreeSize() const;  // Clearly counted value. Ignoring real node positions
    double square() const;

    // Internal counts and preparations
    void setTreeNodeCoors(QPointF leftTopPoint = QPointF());

    PaintedTerm* getNodeAtPoint(const QPointF& pt) const;

    // Deletions
    TermTree(const TermTree&) = delete;

private:    
    // Internal Info
    PaintedTerm::List getAllNodesInTree() const;
    qreal             getMaxStackHeight() const;

private:  // Members
    NodeVerticalStack::List mStacks;
};
