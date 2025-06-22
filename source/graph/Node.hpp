// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <memory>
#include <vector>

namespace graph {

template<typename Data>
class Node
{
public:
    using Ptr  = std::shared_ptr<Node<Data>>;
    using List = std::vector<Ptr>;

    explicit Node(const Data& data)
        : mData(data)
    {}

    static Ptr createPtr(Data data) { return std::make_shared<Node<Data>>(data); }

    void        setData(const Data& data) { mData = data; }
    Data&       data() { return mData; }
    const Data& data() const { return mData; }

private:
    Data mData;
};

} // namespace graph
