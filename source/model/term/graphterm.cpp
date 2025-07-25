// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <vector>

#include "source/graph/Node.hpp"

#include "source/model/term/termdatacache.h"

export module GraphTerm;

// This class contains all "graph" part of term
// like edges, place in graph hierarchy and so on
export class GraphTerm : public graph::Node<TermData>
{
public:
    using Ptr  = std::shared_ptr<GraphTerm>;
    using List = std::vector<Ptr>;

    GraphTerm(const TermData& info)
        : Node<TermData>(info)
        , mDataCache(info)
    {}

    ~GraphTerm() = default;

    const TermDataCache& cache() const { return mDataCache; }

    void addSynonym(const QString& synonym) { mDataCache.addSynonym(synonym); }

private:
    TermDataCache mDataCache;
};
