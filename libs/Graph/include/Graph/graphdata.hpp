// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <functional>
#include <vector>

#include "source/model/base/edge.hpp"
#include "source/model/base/node.hpp"

namespace graph {

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

} // namespace graph
