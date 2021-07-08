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

#include <cassert>
#include <deque>
#include <functional>
#include <map>

#include "source/model/base/graph.hpp"
#include "source/model/enums.h"

template<typename NodeT, typename EdgeT>
class Forest : public Graph<NodeT, EdgeT>
{
private:
    using NodePtr  = NodeT::Ptr;
    using NodeList = NodeT::List;

    using EdgePtr  = EdgeT::Ptr;
    using EdgeList = EdgeT::List;

    using Base     = Graph<NodeT, EdgeT>;
    using BaseData = GraphData<NodeT, EdgeT>;

public:
    using Ptr  = std::shared_ptr<Forest<NodeT, EdgeT>>;
    using List = std::vector<Ptr>;

    explicit Forest(const GraphData<NodeT, EdgeT>& data)
        : Graph<NodeT, EdgeT>({.nodes = data.nodes, .edges = data.edges})
    {
        assert(Base::isolatedNodes().empty());
        rebuildCache();

        mBrokenEdges = getCycleEdges();

        if (!mBrokenEdges.empty()) {
            BaseData::edges = BaseData::subtractEdgeList(BaseData::edges, mBrokenEdges);
            rebuildCache();
        }

        mWasteEdges = getWasteEdges();

        if (!mWasteEdges.empty()) {
            BaseData::edges = BaseData::subtractEdgeList(BaseData::edges, mWasteEdges);
            rebuildCache();
        }

        mLevels = getLevels();

        assert(getCycleEdges().empty());
    }

    bool isRoot(const NodePtr& node) const { return nodeType(node) == NodeType::root; }
    bool isLeaf(const NodePtr& node) const
    {
        auto type = nodeType(node);
        return type == NodeType::endLeaf || type == NodeType::middleLeaf;
    }

    bool     hasBrokenEdges() const { return !mBrokenEdges.empty(); }
    EdgeList brokenEdges() const { return mBrokenEdges; }

    bool     hasWasteEdges() const { return !mWasteEdges.empty(); }
    EdgeList wasteEdges() const { return mWasteEdges; }

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

    void rootsVisiter(const NodePtr&                                  node,
                      const std::function<bool(const NodePtr& node)>& stopCondition,
                      bool                                            checkStartNode = false) const
    {
        std::deque<NodePtr> visitQueue;
        visitQueue.push_back(node);
        nodesVisiter(stopCondition, visitQueue, mEdgesToRoots, checkStartNode);
    }

    void leafsVisiter(const NodePtr&                                  node,
                      const std::function<bool(const NodePtr& node)>& stopCondition,
                      bool                                            checkStartNode = false) const
    {
        std::deque<NodePtr> visitQueue;
        visitQueue.push_back(node);
        nodesVisiter(stopCondition, visitQueue, mEdgesToLeafs, checkStartNode);
    }

    NodeList roots() const
    {
        return BaseData::filterNodes([this](auto node) { return isRoot(node); });
    }

    NodeList rootNodes(const NodePtr& node) const
    {
        NodeList ret;
        std::ranges::transform(mEdgesToRoots.at(node), std::back_inserter(ret), [&node](auto edge) {
            return edge->oppositeTo(node);
        });
        return ret;
    }

    NodeList leafNodes(const NodePtr& node) const
    {
        NodeList ret;
        std::ranges::transform(mEdgesToLeafs.at(node), std::back_inserter(ret), [&node](auto edge) {
            return edge->oppositeTo(node);
        });
        return ret;
    }

    bool isAncestor(const NodePtr& node, const NodePtr& expectedAncestor) const
    {
        bool result = false;
        rootsVisiter(node, [&result, &expectedAncestor](auto node) {
            if (node == expectedAncestor) {
                result = true;
                return true;
            }
            return false;
        });
        return result;
    }

    bool isFarAncestor(const NodePtr& node, const NodePtr& expectedFarAncestor) const
    {
        for (auto rootNode : rootNodes(node)) {
            if (rootNode == expectedFarAncestor)
                continue;

            if (isAncestor(rootNode, expectedFarAncestor)) {
                return true;
            }
        }

        return false;
    }

    EdgeList tooShortEdges(const NodePtr& node) const
    {
        EdgeList ret;

        for (auto edge : mEdgesToRoots.at(node)) {
            // If we found long path, we need mark direct path for cut out
            if (isFarAncestor(node, edge->oppositeTo(node)))
                ret.push_back(edge);
        }

        return ret;
    }

    // Leveling
    int level(const NodePtr& node) const { return mLevels.at(node); }

    NodeList nodesAtLevel(int level) const
    {
        NodeList ret;

        for (const auto item : mLevels) {
            if (item.second == level)
                ret.push_back(item.first);
        }

        return ret;
    }

protected:
    const std::map<NodePtr, EdgeList>& edgesToRoots() { return mEdgesToRoots; }
    const std::map<NodePtr, EdgeList>& edgesToLeafs() { return mEdgesToLeafs; }

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

    EdgeList getWasteEdges()
    {
        std::set<EdgePtr> wEdges;
        for (const auto& node : BaseData::nodes) {
            auto edges = tooShortEdges(node);

            std::ranges::for_each(edges, [&wEdges](auto edge) {
                if (!wEdges.contains(edge))
                    wEdges.insert(edge);
            });
        }
        return {wEdges.begin(), wEdges.end()};
    }

    enum class NodeState { NotVisited = 0, AtPath, Visited };

    EdgeList getCycleEdges() const
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

    std::map<NodePtr, int> getLevels() const
    {
        using namespace std;

        map<NodePtr, int> ret;
        deque<NodePtr>    visitQueue;
        auto              rootNodes = roots();

        ranges::for_each(BaseData::nodes, [&ret](auto node) { ret[node] = 0; });
        ranges::for_each(rootNodes, [&visitQueue](auto node) { visitQueue.push_back(node); });

        while (!visitQueue.empty()) // Just steps limit
        {
            auto node = visitQueue.front();
            visitQueue.pop_front();

            for (const auto leaf : leafNodes(node)) {
                ret[leaf] = max(ret[node] + 1, ret[leaf]);

                if (ranges::find(visitQueue, leaf) == visitQueue.end()) // Not found
                    visitQueue.push_back(leaf);
            }
        }

        return ret;
    }

    void cycleCheckVisit(const NodePtr& node, EdgeList& breakEdges, std::map<NodePtr, NodeState>& nodeStates) const
    {
        nodeStates[node] = NodeState::AtPath;

        for (auto edge : mEdgesToLeafs.at(node)) {
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

    std::map<NodePtr, int> mLevels;

    EdgeList mBrokenEdges; // From broken cycles
    EdgeList mWasteEdges;  // Just redundant
};
