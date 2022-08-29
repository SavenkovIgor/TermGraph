// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <memory>
#include <vector>

#include "source/model/base/edge.hpp"
#include "source/model/edge/edgedata.h"
#include "source/model/term/graphterm.h"

class GraphTerm;

class GraphEdge : public Edge<GraphTerm, EdgeData>
{
    using TermPtr = std::shared_ptr<GraphTerm>;

public:
    using Ptr  = std::shared_ptr<GraphEdge>;
    using List = std::vector<Ptr>;

    GraphEdge(TermPtr toRoot, TermPtr toLeaf);
    virtual ~GraphEdge() = default;
};
