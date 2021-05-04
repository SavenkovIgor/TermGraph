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
#include "source/Model/Base/graphdata.hpp"
#include "source/Model/Base/node.hpp"

template<typename NodeData, typename NodeGraphContext, typename EdgeData, typename EdgeGraphContext>
class Graph
{
    struct NodeWrap
    {
        NodeGraphContext    context;
        Node<NodeData>::Ptr node;
    };

    struct EdgeWrap
    {
        EdgeGraphContext              context;
        Edge<NodeData, EdgeData>::Ptr edge;
    };

public:
    explicit Graph(const GraphData<NodeData, EdgeData>& data)
    {
        for (const auto& node : data.nodes)
            mNodeList.push_back({.context = NodeGraphContext{}, .node = node});

        for (const auto& edge : data.edges)
            mEdgeList.push_back({.context = EdgeGraphContext{}, .edge = edge});
    }

    const NodeGraphContext& nodeContextAt(std::size_t i) const { return mNodeList.at(i).context; }
    Node<NodeData>::Ptr     nodeAt(std::size_t i) const { return mNodeList.at(i).node; }

    const EdgeGraphContext&       edgeContextAt(std::size_t i) const { return mEdgeList.at(i).context; }
    Edge<NodeData, EdgeData>::Ptr edgeAt(std::size_t i) const { return mEdgeList.at(i).edge; }

private:
    std::vector<NodeWrap> mNodeList;
    std::vector<EdgeWrap> mEdgeList;
};
