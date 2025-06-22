// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <memory>
#include <vector>

#include "source/graph/Node.hpp"

namespace graph {

template<typename NodeT, typename EdgeData>
class Edge
{
    using NodePtr = std::shared_ptr<NodeT>;

public:
    using Ptr  = std::shared_ptr<Edge<NodeT, EdgeData>>;
    using List = std::vector<Ptr>;

    explicit Edge(NodePtr root, NodePtr leaf, EdgeData data)
        : mRoot(std::move(root))
        , mLeaf(std::move(leaf))
        , mData(std::move(data))
    {}

    static Ptr createPtr(NodePtr root, NodePtr leaf, EdgeData data = {})
    {
        return std::make_shared<Edge<NodeT, EdgeData>>(root, leaf, data);
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
