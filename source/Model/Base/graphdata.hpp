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

#include <utility>
#include <vector>

#include "source/Model/Base/edge.hpp"
#include "source/Model/Base/node.hpp"

template<typename NodeData, typename EdgeData>
struct GraphData
{
    using List = std::vector<GraphData<NodeData, EdgeData>>;

    Node<NodeData>::List           nodes;
    Edge<NodeData, EdgeData>::List edges;

    bool contains(const Node<NodeData>::Ptr& node) const { return std::ranges::find(nodes, node) != nodes.end(); }

    bool contains(const Edge<NodeData, EdgeData>::Ptr& edge) const
    {
        return std::ranges::find(edges, edge) != edges.end();
    }
};
