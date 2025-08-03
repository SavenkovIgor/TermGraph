// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <memory>
#include <vector>

export module Graph.Node;

using namespace std;

export namespace graph {

template<typename Data>
class Node
{
public:
    using Ptr  = shared_ptr<Node<Data>>;
    using List = vector<Ptr>;

    explicit Node(const Data& data)
        : mData(data)
    {}

    static Ptr createPtr(Data data) { return make_shared<Node<Data>>(data); }

    void        setData(const Data& data) { mData = data; }
    Data&       data() { return mData; }
    const Data& data() const { return mData; }

private:
    Data mData;
};

} // namespace graph
