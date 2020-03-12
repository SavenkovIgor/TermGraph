/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
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

GraphEdge::GraphEdge(GraphTerm* toRoot, GraphTerm* toLeaf)
{
    assert(toRoot != nullptr);
    assert(toLeaf != nullptr);
    assert(toRoot != toLeaf);

    this->toRoot = toRoot;
    this->toLeaf = toLeaf;
}

GraphEdge::GraphEdge()
{
    this->toRoot = nullptr;
    this->toLeaf = nullptr;
}

bool GraphEdge::hasNode(GraphTerm* node) const
{
    if (node == toRoot || node == toLeaf)
        return true;

    return false;
}

bool GraphEdge::isSameEdge(GraphTerm* rt, GraphTerm* br) const
{
    bool rOk = (toRoot == rt || toRoot == br);
    bool bOk = (toLeaf == rt || toLeaf == br);

    return rOk && bOk;
}

GraphTerm* GraphEdge::getOtherSide(GraphTerm* n) const
{
    if (n == toRoot) {
        return toLeaf;
    } else if (n == toLeaf) {
        return toRoot;
    }

    return nullptr;
}
