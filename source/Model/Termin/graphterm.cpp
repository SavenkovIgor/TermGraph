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

#include "source/Model/Termin/graphterm.h"

#include <ranges>

GraphTerm::GraphTerm(const TermData& info)
    : Node<TermData>(info)
    , mDataCache(info)
{}

bool GraphTerm::isRoot() const { return getNodeType() == NodeType::root; }

NodeType GraphTerm::getNodeType() const
{
    if (edgesToRoots.empty()) {
        if (edgesToLeafs.empty()) {
            return NodeType::orphan; // Both empty
        } else {
            return NodeType::root; // No connections down, has connections up
        }
    } else {
        if (edgesToLeafs.empty()) {
            return NodeType::endLeaf; // Has connections down, no connections up
        } else {
            return NodeType::middleLeaf; // Has both connections, up and down
        }
    }
}

void GraphTerm::setTreeId(unsigned int treeId)
{
    if (this->treeId == 0) {
        this->treeId = treeId;
    } else {
        return;
    }

    for (auto edge : getAllConnectedEdges()) {
        edge->oppositeTo(this)->setTreeId(treeId);
    }
}

unsigned int GraphTerm::getTreeId() const { return treeId; }

void GraphTerm::addEdgeRef(GraphEdge::Ptr edge)
{
    // We are source - connection up
    if (edge->root().get() == this && edge->leaf().get() != this)
        edgesToLeafs.push_back(edge);

    // We are acceptor - connection down
    if (edge->leaf().get() == this && edge->root().get() != this)
        edgesToRoots.push_back(edge);
}

GraphEdge::List GraphTerm::getAllConnectedEdges() const
{
    GraphEdge::List ret;

    for (auto e : edgesToRoots)
        ret.push_back(e);

    for (auto e : edgesToLeafs)
        ret.push_back(e);

    return ret;
}

GraphEdge::List GraphTerm::getEdgesToRoots() const { return edgesToRoots; }

const TermDataCache& GraphTerm::cache() const { return mDataCache; }
