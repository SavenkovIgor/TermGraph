// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/model/edge/graphedge.h"

GraphEdge::GraphEdge(TermPtr toRoot, TermPtr toLeaf)
    : graph::Edge<GraphTerm, EdgeData>(toRoot, toLeaf, EdgeData())
{
    assert(toRoot != toLeaf);
}
