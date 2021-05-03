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

#include "source/Model/Base/edge.hpp"

TEST(EdgeTest, InitEdge)
{
    auto n1 = Node<int>::createPtr(1);
    auto n2 = Node<int>::createPtr(2);

    auto edge = Edge<int, int>::createPtr(n1, n2);

    EXPECT_EQ(edge->root()->data(), 1);
    EXPECT_EQ(edge->leaf()->data(), 2);
}

TEST(EdgeTest, EdgeList)
{
    auto n1 = Node<int>::createPtr(1);
    auto n2 = Node<int>::createPtr(2);

    auto e1 = Edge<int, int>::createPtr(n1, n2);
    auto e2 = Edge<int, int>::createPtr(n2, n1);

    Edge<int, int>::List edgeList;

    edgeList.push_back(e1);
    edgeList.push_back(e2);

    EXPECT_EQ(edgeList.size(), 2);

    EXPECT_EQ(edgeList[0]->root()->data(), 1);
    EXPECT_EQ(edgeList[0]->leaf()->data(), 2);

    EXPECT_EQ(edgeList[1]->root()->data(), 2);
    EXPECT_EQ(edgeList[1]->leaf()->data(), 1);
}

TEST(EdgeTest, Data)
{
    auto n1 = Node<int>::createPtr(1);
    auto n2 = Node<int>::createPtr(2);

    auto edge = Edge<int, int>::createPtr(n1, n2, 15);

    EXPECT_EQ(edge->data(), 15);

    edge->setData(55 + 12);

    EXPECT_EQ(edge->data(), 67);
}
