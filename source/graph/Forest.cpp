// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <algorithm>
#include <cassert>
#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <ranges>
#include <set>

#include "source/graph/NodeType.hpp"

export module Graph.Forest;

import Graph.Graph;
import Graph.GraphData;

namespace rng = std::ranges;

export namespace graph {

template<typename NodeT, typename EdgeT>
class Forest : public Graph<NodeT, EdgeT>
{
private:
    using NodePtr = typename NodeT::Ptr;
    using EdgePtr = typename EdgeT::Ptr;

    using Base     = Graph<NodeT, EdgeT>;
    using BaseData = GraphData<NodeT, EdgeT>;

public:
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

    bool isRoot(const NodePtr& node) const { return nodeType(node) == NodeType::Root; }
    bool isLeaf(const NodePtr& node) const
    {
        auto type = nodeType(node);
        return type == NodeType::EndLeaf || type == NodeType::MiddleLeaf;
    }

    bool        hasBrokenEdges() const { return !mBrokenEdges.empty(); }
    EdgeT::List brokenEdges() const { return mBrokenEdges; }

    bool        hasWasteEdges() const { return !mWasteEdges.empty(); }
    EdgeT::List wasteEdges() const { return mWasteEdges; }

    NodeType nodeType(const NodePtr& node) const
    {
        assert(Base::contains(node));

        const auto& edgesToRoots = mEdgesToRoots.at(node);
        const auto& edgesToLeafs = mEdgesToLeafs.at(node);

        if (edgesToRoots.empty()) {
            if (edgesToLeafs.empty()) {
                // Both empty
                return NodeType::Orphan;
            } else {
                // No connections down, has connections up
                return NodeType::Root;
            }
        } else {
            if (edgesToLeafs.empty()) {
                // Has connections down, no connections up
                return NodeType::EndLeaf;
            } else {
                // Has both connections, up and down
                return NodeType::MiddleLeaf;
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

    NodeT::List roots() const
    {
        return BaseData::filterNodes([this](auto node) { return isRoot(node); });
    }

    NodeT::List rootNodes(const NodePtr& node) const
    {
        typename NodeT::List ret;
        auto                 rootEdges = mEdgesToRoots.at(node);
        std::transform(rootEdges.begin(), rootEdges.end(), std::back_inserter(ret), [&node](auto edge) {
            return edge->oppositeTo(node);
        });
        return ret;
    }

    NodeT::List leafNodes(const NodePtr& node) const
    {
        typename NodeT::List ret;
        auto                 leafEdges = mEdgesToLeafs.at(node);
        std::transform(leafEdges.begin(), leafEdges.end(), std::back_inserter(ret), [&node](auto edge) {
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

    EdgeT::List tooShortEdges(const NodePtr& node) const
    {
        typename EdgeT::List ret;

        for (auto edge : mEdgesToRoots.at(node)) {
            // If we found long path, we need mark direct path for cut out
            if (isFarAncestor(node, edge->oppositeTo(node)))
                ret.push_back(edge);
        }

        return ret;
    }

    // Leveling
    int level(const NodePtr& node) const { return mLevels.at(node); }

    NodeT::List nodesAtLevel(int level) const
    {
        typename NodeT::List ret;

        for (const auto& [item, itemLevel] : mLevels) {
            if (itemLevel == level)
                ret.push_back(item);
        }

        return ret;
    }

protected:
    const std::map<NodePtr, typename EdgeT::List>& edgesToRoots() { return mEdgesToRoots; }
    const std::map<NodePtr, typename EdgeT::List>& edgesToLeafs() { return mEdgesToLeafs; }

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

    EdgeT::List getWasteEdges()
    {
        std::set<EdgePtr> wEdges;
        for (const auto& node : BaseData::nodes) {
            auto edges = tooShortEdges(node);

            rng::for_each(edges, [&wEdges](auto edge) {
                if (!wEdges.contains(edge))
                    wEdges.insert(edge);
            });
        }
        return {wEdges.begin(), wEdges.end()};
    }

    enum class NodeState { NotVisited = 0, AtPath, Visited };

    typename EdgeT::List getCycleEdges() const
    {
        typename EdgeT::List breakEdges;

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
        std::map<NodePtr, int> ret;
        std::deque<NodePtr>    visitQueue;
        auto                   rootNodes = roots();

        rng::for_each(BaseData::nodes, [&ret](auto node) { ret[node] = 0; });
        rng::for_each(rootNodes, [&visitQueue](auto node) { visitQueue.push_back(node); });

        while (!visitQueue.empty()) // Just steps limit
        {
            auto node = visitQueue.front();
            visitQueue.pop_front();

            for (const auto& leaf : leafNodes(node)) {
                ret[leaf] = std::max(ret[node] + 1, ret[leaf]);

                if (rng::find(visitQueue, leaf) == visitQueue.end()) // Not found
                    visitQueue.push_back(leaf);
            }
        }

        // Roots offset
        int maxLevel = 0;

        for (const auto& [_, level] : ret)
            maxLevel = std::max(maxLevel, level);

        for (const auto& root : rootNodes) {
            auto leafs = leafNodes(root);

            int min = maxLevel;
            for (const auto& leaf : leafs)
                min = std::min(ret[leaf], min);

            ret[root] = std::max(min - 1, 0);
        }

        return ret;
    }

    void cycleCheckVisit(const NodePtr& node, EdgeT::List& breakEdges, std::map<NodePtr, NodeState>& nodeStates) const
    {
        nodeStates[node] = NodeState::AtPath;

        for (auto edge : mEdgesToLeafs.at(node)) {
            auto iter = rng::find(breakEdges, edge);
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
                             const std::map<NodePtr, typename EdgeT::List>&  edgesList,
                             bool                                            checkCondition = true)
    {
        if (visitQueue.empty())
            return;

        auto node = visitQueue.front();
        visitQueue.pop_front();

        if (checkCondition) {
            if (stopCondition(node)) {
                return;
            }
        }

        for (const auto& edge : edgesList.at(node)) {
            auto rootNode = edge->oppositeTo(node);

            bool found = rng::find(visitQueue, rootNode) != visitQueue.end();

            if (!found)
                visitQueue.push_back(rootNode);
        }

        nodesVisiter(stopCondition, visitQueue, edgesList);
    }

private: // Members
    std::map<NodePtr, typename EdgeT::List> mEdgesToRoots;
    std::map<NodePtr, typename EdgeT::List> mEdgesToLeafs;

    std::map<NodePtr, int> mLevels;

    typename EdgeT::List mBrokenEdges; // From broken cycles
    typename EdgeT::List mWasteEdges;  // Just redundant
};

} // namespace graph
