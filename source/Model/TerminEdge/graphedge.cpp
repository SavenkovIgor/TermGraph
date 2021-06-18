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

#include "source/Model/TerminEdge/graphedge.h"

#include "source/Model/Termin/graphterm.h"

GraphEdge::GraphEdge(TermPtr toRoot, TermPtr toLeaf)
{
    assert(toRoot != toLeaf);

    this->toRoot = toRoot;
    this->toLeaf = toLeaf;

    toRoot->addEdgeRef(this);
    toLeaf->addEdgeRef(this);
}

GraphEdge::GraphEdge()
{
    this->toRoot = nullptr;
    this->toLeaf = nullptr;
}

bool GraphEdge::hasNode(GraphTerm* node) const
{
    if (node == toRoot.get() || node == toLeaf.get())
        return true;

    return false;
}

bool GraphEdge::isSameEdge(GraphTerm* rt, GraphTerm* br) const
{
    bool rOk = (toRoot.get() == rt || toRoot.get() == br);
    bool bOk = (toLeaf.get() == rt || toLeaf.get() == br);

    return rOk && bOk;
}

GraphTerm* GraphEdge::getOtherSide(GraphTerm* n) const
{
    if (n == toRoot.get()) {
        return toLeaf.get();
    } else if (n == toLeaf.get()) {
        return toRoot.get();
    }

    return nullptr;
}
