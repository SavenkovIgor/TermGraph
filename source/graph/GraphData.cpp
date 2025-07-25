// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <functional>
#include <ranges>
#include <vector>

#include "source/graph/Edge.hpp"
#include "source/graph/Node.hpp"

export module Graph.GraphData;

namespace rng = std::ranges;

export namespace graph {

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

    bool contains(const NodePtr& node) const { return rng::find(nodes, node) != nodes.end(); }
    bool contains(const EdgePtr& edge) const { return rng::find(edges, edge) != edges.end(); }

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
            return rng::find(subtractor, node) != subtractor.end();
        });

        ret.erase(remIt, ret.end());

        return ret;
    }

    static EdgeList subtractEdgeList(const EdgeList& baseList, const EdgeList& subtractor)
    {
        EdgeList ret = baseList;

        auto remIt = std::remove_if(ret.begin(), ret.end(), [&subtractor](auto node) {
            return rng::find(subtractor, node) != subtractor.end();
        });

        ret.erase(remIt, ret.end());

        return ret;
    }
};

} // namespace graph
