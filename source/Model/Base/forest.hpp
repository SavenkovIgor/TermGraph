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

#include <deque>
#include <functional>
#include <map>

#include "source/Model/Base/graph.hpp"
#include "source/Model/enums.h"

template<typename NodeData, typename EdgeData>
class Forest : protected Graph<NodeData, EdgeData>
{
private:
    using NodePtr  = Node<NodeData>::Ptr;
    using NodeList = Node<NodeData>::List;

    using EdgePtr  = Edge<NodeData, EdgeData>::Ptr;
    using EdgeList = Edge<NodeData, EdgeData>::List;

    using Base     = Graph<NodeData, EdgeData>;
    using BaseData = GraphData<NodeData, EdgeData>;

public:
    explicit Forest(const GraphData<NodeData, EdgeData>& data)
        : Graph<NodeData, EdgeData>({.nodes = data.nodes, .edges = data.edges})
    {
        assert(Base::isolatedNodes().empty());
        rebuildCache();

        auto brokeEdges = getCycleEdges();

        auto remIt = std::remove_if(BaseData::edges.begin(), BaseData::edges.end(), [&brokeEdges](auto edge) {
            return std::ranges::find(brokeEdges, edge) != brokeEdges.end();
        });

        BaseData::edges.erase(remIt, BaseData::edges.end());

        mBrokenEdges = brokeEdges;

        if (!mBrokenEdges.empty())
            rebuildCache();

        assert(getCycleEdges().empty());
    }

    bool isRoot(const NodePtr& node) const { return nodeType(node) == NodeType::root; }
    bool isOrphan(const NodePtr& node) const { return nodeType(node) == NodeType::orphan; }
    bool isLeaf(const NodePtr& node) const
    {
        auto type = nodeType(node);
        return type == NodeType::endLeaf || type == NodeType::middleLeaf;
    }

    bool isInTree(const NodePtr& node) const { return !isOrphan(node); }

    bool     hasBrokenEdges() const { return !mBrokenEdges.empty(); }
    EdgeList brokenEdges() const { return mBrokenEdges; }

    NodeType nodeType(const NodePtr& node) const
    {
        assert(Base::contains(node));

        const auto& edgesToRoots = mEdgesToRoots.at(node);
        const auto& edgesToLeafs = mEdgesToLeafs.at(node);

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

    void rootsVisiter(const NodePtr& node, const std::function<bool(const NodePtr& node)>& stopCondition)
    {
        std::deque<NodePtr> visitQueue;
        visitQueue.push_back(node);
        nodesVisiter(stopCondition, visitQueue, mEdgesToRoots, false);
        assert(visitQueue.empty());
    }

    void leafsVisiter(const NodePtr& node, const std::function<bool(const NodePtr& node)>& stopCondition)
    {
        std::deque<NodePtr> visitQueue;
        visitQueue.push_back(node);
        nodesVisiter(stopCondition, visitQueue, mEdgesToLeafs, false);
        assert(visitQueue.empty());
    }

private: // Methods
    void rebuildCache()
    {
        mEdgesToRoots.clear();
        mEdgesToLeafs.clear();

        for (const auto& node : BaseData::nodes) {
            mEdgesToLeafs[node] = {};
            mEdgesToRoots[node] = {};

            for (const auto& edge : Base::connectedEdges(node)) {
                if (edge->root() == node)
                    mEdgesToLeafs[node].push_back(edge);
                else
                    mEdgesToRoots[node].push_back(edge);
            }
        }
    }

    enum class NodeState { NotVisited = 0, AtPath, Visited };

    EdgeList getCycleEdges()
    {
        EdgeList breakEdges;

        std::map<NodePtr, NodeState> nodeStates;

        for (const auto& node : BaseData::nodes)
            nodeStates[node] = NodeState::NotVisited;

        // First find all edges to break
        for (const auto& node : BaseData::nodes)
            cycleCheckVisit(node, breakEdges, nodeStates);

        return breakEdges;
    }

    void cycleCheckVisit(const NodePtr& node, EdgeList& breakEdges, std::map<NodePtr, NodeState>& nodeStates)
    {
        nodeStates[node] = NodeState::AtPath;

        for (auto edge : mEdgesToLeafs[node]) {
            auto iter = std::ranges::find(breakEdges, edge);
            if (iter != breakEdges.end())
                continue;

            auto leafNode = edge->oppositeTo(node);

            if (nodeStates[leafNode] == NodeState::AtPath) {
                breakEdges.push_back(edge);
            } else {
                if (nodeStates[leafNode] == NodeState::NotVisited) {
                    cycleCheckVisit(leafNode, breakEdges, nodeStates);
                }
            }
        }

        nodeStates[node] = NodeState::Visited;
    }

    static void nodesVisiter(const std::function<bool(const NodePtr& node)>& stopCondition,
                             std::deque<NodePtr>&                            visitQueue,
                             const std::map<NodePtr, EdgeList>&              edgesList,
                             bool                                            checkCondition = true)
    {
        using namespace std;

        if (visitQueue.empty())
            return;

        auto node = visitQueue.front();
        visitQueue.pop_front();

        if (checkCondition) {
            if (stopCondition(node)) {
                return;
            }
        }

        for (const auto edge : edgesList.at(node)) {
            auto rootNode = edge->oppositeTo(node);

            bool found = ranges::find(visitQueue, rootNode) != visitQueue.end();

            if (!found)
                visitQueue.push_back(rootNode);
        }

        nodesVisiter(stopCondition, visitQueue, edgesList);
    }

private: // Members
    std::map<NodePtr, EdgeList> mEdgesToRoots;
    std::map<NodePtr, EdgeList> mEdgesToLeafs;

    EdgeList mBrokenEdges;
};
