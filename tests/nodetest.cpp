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

#include <gtest/gtest.h>

#include "source/Model/Base/node.hpp"

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
