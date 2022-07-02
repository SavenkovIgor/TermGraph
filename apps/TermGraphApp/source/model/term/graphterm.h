// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <vector>

#include "source/model/base/node.hpp"
#include "source/model/edge/graphedge.h"
#include "source/model/enums.h"
#include "source/model/term/termdatacache.h"

// This class contains all "graph" part of term
// like edges, place in graph hierarchy and so on
class GraphTerm : public Node<TermData>
{
public:
    using Ptr  = std::shared_ptr<GraphTerm>;
    using List = std::vector<Ptr>;

    GraphTerm(const TermData& info);
    ~GraphTerm() = default;

    const TermDataCache& cache() const;

private:
    const TermDataCache mDataCache;
};
