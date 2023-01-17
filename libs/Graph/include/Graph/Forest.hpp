// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <cassert>
#include <deque>
#include <functional>
#include <map>

#include <Graph/Graph.hpp>

namespace graph {

enum class NodeType { Orphan, Root, EndLeaf, MiddleLeaf };

template<typename NodeT, typename EdgeT>
class Forest : public Graph<NodeT, EdgeT>
{
private:
    using NodePtr  = typename NodeT::Ptr;
    using NodeList = typename NodeT::List;

    using EdgePtr  = typename EdgeT::Ptr;
    using EdgeList = typename EdgeT::List;

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

    bool isRoot(const NodePtr& node) const { return nodeType(node) == NodeType::Root; }
    bool isLeaf(const NodePtr& node) const
    {
        auto type = nodeType(node);
        return type == NodeType::EndLeaf || type == NodeType::MiddleLeaf;
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
                return NodeType::Orphan; // Both empty
            } else {
                return NodeType::Root; // No connections down, has connections up
            }
        } else {
            if (edgesToLeafs.empty()) {
                return NodeType::EndLeaf; // Has connections down, no connections up
            } else {
                return NodeType::MiddleLeaf; // Has both connections, up and down
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
        auto     rootEdges = mEdgesToRoots.at(node);
        std::transform(rootEdges.begin(), rootEdges.end(), std::back_inserter(ret), [&node](auto edge) {
            return edge->oppositeTo(node);
        });
        return ret;
    }

    NodeList leafNodes(const NodePtr& node) const
    {
        NodeList ret;
        auto     leafEdges = mEdgesToLeafs.at(node);
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

        for (const auto& [item, itemLevel] : mLevels) {
            if (itemLevel == level)
                ret.push_back(item);
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

            std::for_each(edges.begin(), edges.end(), [&wEdges](auto edge) {
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

        for_each(BaseData::nodes.begin(), BaseData::nodes.end(), [&ret](auto node) { ret[node] = 0; });
        for_each(rootNodes.begin(), rootNodes.end(), [&visitQueue](auto node) { visitQueue.push_back(node); });

        while (!visitQueue.empty()) // Just steps limit
        {
            auto node = visitQueue.front();
            visitQueue.pop_front();

            for (const auto& leaf : leafNodes(node)) {
                ret[leaf] = max(ret[node] + 1, ret[leaf]);

                if (find(visitQueue.begin(), visitQueue.end(), leaf) == visitQueue.end()) // Not found
                    visitQueue.push_back(leaf);
            }
        }

        // Roots offset
        int maxLevel = 0;

        for (const auto&[_, level] : ret)
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

    void cycleCheckVisit(const NodePtr& node, EdgeList& breakEdges, std::map<NodePtr, NodeState>& nodeStates) const
    {
        nodeStates[node] = NodeState::AtPath;

        for (auto edge : mEdgesToLeafs.at(node)) {
            auto iter = std::find(breakEdges.begin(), breakEdges.end(), edge);
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

        for (const auto& edge : edgesList.at(node)) {
            auto rootNode = edge->oppositeTo(node);

            bool found = find(visitQueue.begin(), visitQueue.end(), rootNode) != visitQueue.end();

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

}
