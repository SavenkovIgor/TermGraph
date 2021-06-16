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

#include <memory>
#include <vector>

#include "source/Model/Base/node.hpp"

template<typename NodeData, typename EdgeData>
class Edge
{
    using NodePtr = Node<NodeData>::Ptr;

public:
    using Ptr  = std::shared_ptr<Edge<EdgeData, NodeData>>;
    using List = std::vector<Ptr>;

    explicit Edge(NodePtr root, NodePtr leaf, EdgeData data)
        : mRoot(root)
        , mLeaf(leaf)
        , mData(data)
    {}

    static Ptr createPtr(NodePtr root, NodePtr leaf, EdgeData data = {})
    {
        return std::make_shared<Edge<NodeData, EdgeData>>(root, leaf, data);
    }

    NodePtr root() { return mRoot; }
    NodePtr leaf() { return mLeaf; }

    bool incidentalTo(const NodePtr& node) const { return mRoot == node || mLeaf == node; }

    NodePtr oppositeTo(const NodePtr& node) const
    {
        assert(incidentalTo(node));
        return mRoot == node ? mLeaf : mRoot;
    }

    void            setData(const EdgeData& data) { mData = data; }
    EdgeData&       data() { return mData; }
    const EdgeData& data() const { return mData; }

private:
    NodePtr mRoot;
    NodePtr mLeaf;

    EdgeData mData;
};
