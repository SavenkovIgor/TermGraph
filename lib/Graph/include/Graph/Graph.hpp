// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <algorithm>
#include <map>
#include <ranges>
#include <set>
#include <utility>
#include <vector>

#include <Graph/Edge.hpp>
#include <Graph/GraphData.hpp>
#include <Graph/Node.hpp>

namespace rng = std::ranges;

namespace graph {

template<typename NodeT, typename EdgeT>
class Graph : protected GraphData<NodeT, EdgeT>
{
private:
    using NodePtr  = typename NodeT::Ptr;
    using NodeList = typename NodeT::List;

    using EdgePtr  = typename EdgeT::Ptr;
    using EdgeList = typename EdgeT::List;

    using Base = GraphData<NodeT, EdgeT>;

public:
    using List = std::vector<std::shared_ptr<Graph<NodeT, EdgeT>>>;

    explicit Graph(const GraphData<NodeT, EdgeT>& data)
        : GraphData<NodeT, EdgeT>{.nodes = data.nodes, .edges = data.edges}
    {
        // Uniqueness check
        std::set<NodePtr> nodeSet(data.nodes.begin(), data.nodes.end());
        std::set<EdgePtr> edgeSet(data.edges.begin(), data.edges.end());

        assert(rng::size(nodeSet) == rng::size(data.nodes));
        assert(rng::size(edgeSet) == rng::size(data.edges));
    }

    const NodeList& nodeList() const { return Base::nodes; }
    const EdgeList& edgeList() const { return Base::edges; }

    bool contains(const NodePtr& node) const { return Base::contains(node); }
    bool contains(const EdgePtr& edge) const { return Base::contains(edge); }

    NodeList isolatedNodes() const
    {
        auto ret = Base::nodes;

        auto removeIt = std::remove_if(ret.begin(), ret.end(), [this](auto node) {
            auto it = std::find_if(Base::edges.begin(), Base::edges.end(), [&node](auto edge) {
                return edge->incidentalTo(node);
            });

            return it != Base::edges.end();
        });

        ret.erase(removeIt, ret.end());

        return ret;
    }

    NodeList connectedNodes() const
    {
        auto isolated = isolatedNodes();
        auto ret      = Base::nodes;

        auto removeIt = std::remove_if(ret.begin(), ret.end(), [&isolated](auto node) {
            auto it = std::find(isolated.begin(), isolated.end(), node);

            return it != isolated.end();
        });

        ret.erase(removeIt, ret.end());

        return ret;
    }

    EdgeList connectedEdges(const NodePtr& node)
    {
        assert(contains(node));

        EdgeList ret;

        for (auto edge : Base::edges)
            if (edge->incidentalTo(node))
                ret.push_back(edge);

        return ret;
    }

    GraphData<NodeT, EdgeT> surrounding(const NodePtr& node)
    {
        assert(contains(node));
        auto     edges = connectedEdges(node);
        NodeList nodes(edges.size());

        std::transform(edges.begin(), edges.end(), nodes.begin(), [&node](auto edge) { return edge->oppositeTo(node); });

        return {.nodes = nodes, .edges = edges};
    }

    typename GraphData<NodeT, EdgeT>::List bondedSubgraphs()
    {
        using namespace std;

        enum class State { NotVisited = 0, Planned, Visited };
        map<NodePtr, State> nodesVisitList;

        for (const auto& node : connectedNodes())
            nodesVisitList[node] = State::NotVisited;

        if (nodesVisitList.empty())
            return {};

        typename GraphData<NodeT, EdgeT>::List ret;

        auto isNodePlanned = [](const auto& val) { return val.second == State::Planned; };
        auto isNodeVisited = [](const auto& val) { return val.second == State::Visited; };

        set<NodePtr> uniqueNodes;
        set<EdgePtr> uniqueEdges;

        while (!nodesVisitList.empty()) {
            // Visit stage
            auto nodeToVisit = find_if(nodesVisitList.begin(), nodesVisitList.end(), isNodePlanned);

            // If no any nodes in plan, take first
            if (nodeToVisit == nodesVisitList.end())
                nodeToVisit = nodesVisitList.begin();

            auto currentNode = (*nodeToVisit).first;

            nodesVisitList[currentNode] = State::Visited;
            uniqueNodes.insert(currentNode);

            auto neighbours = surrounding(currentNode);

            for (const auto& neighbourNode : neighbours.nodes) {
                if (nodesVisitList[neighbourNode] != State::Visited)
                    nodesVisitList[neighbourNode] = State::Planned;
            }

            for_each(neighbours.edges.begin(), neighbours.edges.end(), [&uniqueEdges](auto edge) {
                uniqueEdges.insert(edge);
            });

            // Cut stage
            auto plannedCount = count_if(nodesVisitList.begin(), nodesVisitList.end(), isNodePlanned);
            auto visitedCount = count_if(nodesVisitList.begin(), nodesVisitList.end(), isNodeVisited);

            if (plannedCount == 0 && visitedCount > 0) {
                // Nodes preparations
                ret.push_back({.nodes = NodeList(uniqueNodes.begin(), uniqueNodes.end()),
                               .edges = EdgeList(uniqueEdges.begin(), uniqueEdges.end())});

                uniqueNodes.clear();
                uniqueEdges.clear();

                // Remove visited from nodesVisitList
                erase_if(nodesVisitList, isNodeVisited);
            }
        }

        return ret;
    }
};

} // namespace graph
