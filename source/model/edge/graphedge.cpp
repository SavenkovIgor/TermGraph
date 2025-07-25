// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <memory>
#include <vector>

#include "source/model/edge/edgedata.h"
#include "source/model/term/graphterm.h"

export module GraphEdge;

import Graph.Edge;

export class GraphEdge : public graph::Edge<GraphTerm, EdgeData>
{
    using TermPtr = std::shared_ptr<GraphTerm>;

public:
    using Ptr  = std::shared_ptr<GraphEdge>;
    using List = std::vector<Ptr>;

    GraphEdge(TermPtr toRoot, TermPtr toLeaf)
        : graph::Edge<GraphTerm, EdgeData>(toRoot, toLeaf, EdgeData())
    {
        assert(toRoot != toLeaf);
    }

    virtual ~GraphEdge() = default;
};
