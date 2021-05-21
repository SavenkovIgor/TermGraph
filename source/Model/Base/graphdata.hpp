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

#include <functional>
#include <ranges>
#include <vector>

#include "source/Model/Base/edge.hpp"
#include "source/Model/Base/node.hpp"

template<typename NodeData, typename EdgeData>
struct GraphData
{
private:
    using NodePtr  = Node<NodeData>::Ptr;
    using NodeList = Node<NodeData>::List;
    using EdgePtr  = Edge<NodeData, EdgeData>::Ptr;
    using EdgeList = Edge<NodeData, EdgeData>::List;

public:
    using List = std::vector<GraphData<NodeData, EdgeData>>;

    NodeList nodes;
    EdgeList edges;

    bool contains(const NodePtr& node) const { return std::ranges::find(nodes, node) != nodes.end(); }
    bool contains(const EdgePtr& edge) const { return std::ranges::find(edges, edge) != edges.end(); }

    NodeList filterNodes(std::function<bool(const NodePtr&)> condition) const
    {
        auto view = nodes | std::views::filter(condition);
        return {view.begin(), view.end()};
    }

    EdgeList filterEdges(std::function<bool(const EdgePtr&)> condition) const
    {
        auto view = edges | std::views::filter(condition);
        return {view.begin(), view.end()};
    }

    static NodeList subtractNodeList(const NodeList& baseList, const NodeList& subtractor)
    {
        NodeList ret = baseList;

        auto remIt = std::remove_if(ret.begin(), ret.end(), [&subtractor](auto node) {
            return std::ranges::find(subtractor, node) != subtractor.end();
        });

        ret.erase(remIt, ret.end());

        return ret;
    }

    static EdgeList subtractEdgeList(const EdgeList& baseList, const EdgeList& subtractor)
    {
        EdgeList ret = baseList;

        auto remIt = std::remove_if(ret.begin(), ret.end(), [&subtractor](auto node) {
            return std::ranges::find(subtractor, node) != subtractor.end();
        });

        ret.erase(remIt, ret.end());

        return ret;
    }
};
