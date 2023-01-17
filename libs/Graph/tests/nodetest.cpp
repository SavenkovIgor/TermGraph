// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include <Graph/Node.hpp>

using namespace graph;

TEST(NodeTest, PrimitiveInit)
{
    Node<int>  node1(12);
    Node<bool> node2(false);

    EXPECT_EQ(node1.data(), 12);
    EXPECT_FALSE(node2.data());

    node1.setData(100);
    node2.setData(true);

    EXPECT_EQ(node1.data(), 100);
    EXPECT_TRUE(node2.data());
}

TEST(NodeTest, StructInit)
{
    struct TstStruct
    {
        int a;
        int b;
    };

    TstStruct a{.a = 14, .b = 42};

    Node<TstStruct> node1(a);

    EXPECT_EQ(node1.data().a, 14);
    EXPECT_EQ(node1.data().b, 42);

    a.a = -1;
    a.b = -2;

    node1.setData(a);

    EXPECT_EQ(node1.data().a, -1);
    EXPECT_EQ(node1.data().b, -2);
}

TEST(NodeTest, ListTest)
{
    auto n1 = Node<int>::createPtr(1);
    auto n2 = Node<int>::createPtr(2);

    Node<int>::List lst;
    lst.push_back(n1);
    lst.push_back(n2);

    EXPECT_EQ(lst.size(), 2);
    EXPECT_EQ(lst[0]->data(), 1);
    EXPECT_EQ(lst[1]->data(), 2);
}
