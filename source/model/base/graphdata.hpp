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
#include <vector>

#include "source/model/base/edge.hpp"
#include "source/model/base/node.hpp"

template<typename NodeT, typename EdgeT>
struct GraphData
{
private:
    using NodePtr  = typename NodeT::Ptr;
    using NodeList = typename NodeT::List;
    using EdgePtr  = typename EdgeT::Ptr;
    using EdgeList = typename EdgeT::List;

public:
    using List = std::vector<GraphData<NodeT, EdgeT>>;

    NodeList nodes;
    EdgeList edges;

    bool contains(const NodePtr& node) const { return std::find(nodes.begin(), nodes.end(), node) != nodes.end(); }
    bool contains(const EdgePtr& edge) const { return std::find(edges.begin(), edges.end(), edge) != edges.end(); }

    NodeList filterNodes(std::function<bool(const NodePtr&)> condition) const
    {
        NodeList ret;
        for (auto node : nodes)
            if (condition(node))
                ret.push_back(node);

        return ret;
    }

    EdgeList filterEdges(std::function<bool(const EdgePtr&)> condition) const
    {
        EdgeList ret;
        for (auto edge : edges)
            if (condition(edge))
                ret.push_back(edge);

        return ret;
    }

    static NodeList subtractNodeList(const NodeList& baseList, const NodeList& subtractor)
    {
        NodeList ret = baseList;

        auto remIt = std::remove_if(ret.begin(), ret.end(), [&subtractor](auto node) {
            return std::find(subtractor.begin(), subtractor.end(), node) != subtractor.end();
        });

        ret.erase(remIt, ret.end());

        return ret;
    }

    static EdgeList subtractEdgeList(const EdgeList& baseList, const EdgeList& subtractor)
    {
        EdgeList ret = baseList;

        auto remIt = std::remove_if(ret.begin(), ret.end(), [&subtractor](auto node) {
            return std::find(subtractor.begin(), subtractor.end(), node) != subtractor.end();
        });

        ret.erase(remIt, ret.end());

        return ret;
    }
};
