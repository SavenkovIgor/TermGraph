// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <cassert>
#include <memory>
#include <vector>

export module Graph.Edge;

import Graph.Node;

using namespace std;

export namespace graph {

template<typename NodeT, typename EdgeData>
class Edge
{
    using NodePtr = shared_ptr<NodeT>;

public:
    using Ptr  = shared_ptr<Edge<NodeT, EdgeData>>;
    using List = vector<Ptr>;

    explicit Edge(NodePtr root, NodePtr leaf, EdgeData data)
        : mRoot(std::move(root))
        , mLeaf(std::move(leaf))
        , mData(std::move(data))
    {}

    static Ptr createPtr(NodePtr root, NodePtr leaf, EdgeData data = {})
    {
        return make_shared<Edge<NodeT, EdgeData>>(root, leaf, data);
    }

    NodePtr root() { return mRoot; }
    NodePtr leaf() { return mLeaf; }

    bool incidentalTo(const NodePtr& node) const { return mRoot == node || mLeaf == node; }
    bool incidentalTo(const NodeT* node) const { return mRoot.get() == node || mLeaf.get() == node; }

    NodePtr oppositeTo(const NodePtr& node) const
    {
        assert(incidentalTo(node));
        return mRoot == node ? mLeaf : mRoot;
    }

    NodePtr oppositeTo(const NodeT* node) const
    {
        assert(incidentalTo(node));
        return mRoot.get() == node ? mLeaf : mRoot;
    }

    void            setData(const EdgeData& data) { mData = data; }
    EdgeData&       data() { return mData; }
    const EdgeData& data() const { return mData; }

private:
    NodePtr mRoot;
    NodePtr mLeaf;

    EdgeData mData;
};

} // namespace graph
