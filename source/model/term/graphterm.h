// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <vector>

#include "source/graph/Node.hpp"

#include "source/model/term/termdatacache.h"

// This class contains all "graph" part of term
// like edges, place in graph hierarchy and so on
class GraphTerm : public graph::Node<TermData>
{
public:
    using Ptr  = std::shared_ptr<GraphTerm>;
    using List = std::vector<Ptr>;

    GraphTerm(const TermData& info);
    ~GraphTerm() = default;

    const TermDataCache& cache() const;

    void addSynonym(const QString& synonym);

private:
    TermDataCache mDataCache;
};
