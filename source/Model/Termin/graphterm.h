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

#include "source/Model/Base/node.hpp"
#include "source/Model/Termin/termdatacache.h"
#include "source/Model/TerminEdge/graphedge.h"
#include "source/Model/enums.h"

// This class contains all "graph" part of term
// like edges, place in graph hierarchy and so on
class GraphTerm : public Node<TermData>
{
public:
    using Ptr  = std::shared_ptr<GraphTerm>;
    using List = std::vector<Ptr>;

    GraphTerm(const TermData& info);
    ~GraphTerm() = default;

    bool isRoot() const;

    NodeType getNodeType() const;

    void         setTreeId(unsigned int treeId);
    unsigned int getTreeId() const;

    void addEdgeRef(GraphEdge::Ptr edge);

    // Delete section
    GraphEdge::List getAllConnectedEdges() const;
    GraphEdge::List getEdgesToRoots() const;

    const TermDataCache& cache() const;

private:
    GraphEdge::List edgesToLeafs;
    GraphEdge::List edgesToRoots;

    unsigned int treeId = 0; // 0 = no value

    const TermDataCache mDataCache;
};
