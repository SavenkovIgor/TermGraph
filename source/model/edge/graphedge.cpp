// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <cassert>
#include <memory>
#include <vector>

export module GraphEdge;

import EdgeData;
import Graph;
import GraphTerm;

using namespace std;

export class GraphEdge : public graph::Edge<GraphTerm, EdgeData>
{
    using TermPtr = shared_ptr<GraphTerm>;

public:
    using Ptr  = shared_ptr<GraphEdge>;
    using List = vector<Ptr>;

    GraphEdge(TermPtr toRoot, TermPtr toLeaf)
        : graph::Edge<GraphTerm, EdgeData>(toRoot, toLeaf, EdgeData())
    {
        assert(toRoot != toLeaf);
    }

    virtual ~GraphEdge() = default;
};
