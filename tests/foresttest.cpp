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

#include "source/Model/Base/forest.hpp"
#include "source/Model/Base/graph.hpp"
#include "source/Model/Base/graphdata.hpp"

class ForestTest : public ::testing::Test
{
public:
    static const Node<int>::Ptr n1;
    static const Node<int>::Ptr n2;
    static const Node<int>::Ptr n3;
    static const Node<int>::Ptr n4;
    static const Node<int>::Ptr n5;
    static const Node<int>::Ptr n6;
    static const Node<int>::Ptr n7;
    static const Node<int>::Ptr n8;

    static const Edge<int, int>::Ptr e1;
    static const Edge<int, int>::Ptr e2;
    static const Edge<int, int>::Ptr e3;
    static const Edge<int, int>::Ptr e4;
    static const Edge<int, int>::Ptr e5;
    static const Edge<int, int>::Ptr e6;
    static const Edge<int, int>::Ptr e7;
    static const Edge<int, int>::Ptr e8;
    static const Edge<int, int>::Ptr e9;
};

const Node<int>::Ptr ForestTest::n1 = Node<int>::createPtr(1);
const Node<int>::Ptr ForestTest::n2 = Node<int>::createPtr(2);
const Node<int>::Ptr ForestTest::n3 = Node<int>::createPtr(3);
const Node<int>::Ptr ForestTest::n4 = Node<int>::createPtr(4);
const Node<int>::Ptr ForestTest::n5 = Node<int>::createPtr(5);
const Node<int>::Ptr ForestTest::n6 = Node<int>::createPtr(6);
const Node<int>::Ptr ForestTest::n7 = Node<int>::createPtr(7);
const Node<int>::Ptr ForestTest::n8 = Node<int>::createPtr(8);

const Edge<int, int>::Ptr ForestTest::e1 = Edge<int, int>::createPtr(n1, n3, 1);
const Edge<int, int>::Ptr ForestTest::e2 = Edge<int, int>::createPtr(n1, n4, 2);
const Edge<int, int>::Ptr ForestTest::e3 = Edge<int, int>::createPtr(n1, n5, 3);
const Edge<int, int>::Ptr ForestTest::e4 = Edge<int, int>::createPtr(n2, n3, 4);
const Edge<int, int>::Ptr ForestTest::e5 = Edge<int, int>::createPtr(n2, n4, 5);
const Edge<int, int>::Ptr ForestTest::e6 = Edge<int, int>::createPtr(n5, n6, 6);
const Edge<int, int>::Ptr ForestTest::e7 = Edge<int, int>::createPtr(n5, n7, 7);
const Edge<int, int>::Ptr ForestTest::e8 = Edge<int, int>::createPtr(n6, n8, 8);
const Edge<int, int>::Ptr ForestTest::e9 = Edge<int, int>::createPtr(n7, n8, 9);

TEST_F(ForestTest, ForestInit)
{
    auto f1 = Forest<int, int>(
        {.nodes = {n1, n2, n3, n4, n5, n6, n7, n8}, .edges = {e1, e2, e3, e4, e5, e6, e7, e8, e9}});
}

TEST_F(ForestTest, NodeTypes)
{
    auto f1 = Forest<int, int>(
        {.nodes = {n1, n2, n3, n4, n5, n6, n7, n8}, .edges = {e1, e2, e3, e4, e5, e6, e7, e8, e9}});

    EXPECT_EQ(f1.nodeType(n1), NodeType::root);
    EXPECT_EQ(f1.nodeType(n2), NodeType::root);
    EXPECT_EQ(f1.nodeType(n3), NodeType::endLeaf);
    EXPECT_EQ(f1.nodeType(n4), NodeType::endLeaf);
    EXPECT_EQ(f1.nodeType(n5), NodeType::middleLeaf);
    EXPECT_EQ(f1.nodeType(n6), NodeType::middleLeaf);
    EXPECT_EQ(f1.nodeType(n7), NodeType::middleLeaf);
    EXPECT_EQ(f1.nodeType(n8), NodeType::endLeaf);
}

TEST_F(ForestTest, NodeTypes2)
{
    auto f1 = Forest<int, int>(
        {.nodes = {n1, n2, n3, n4, n5, n6, n7, n8}, .edges = {e1, e2, e3, e4, e5, e6, e7, e8, e9}});

    EXPECT_TRUE(f1.isRoot(n1));
    EXPECT_TRUE(f1.isRoot(n2));
    EXPECT_TRUE(f1.isLeaf(n3));
    EXPECT_TRUE(f1.isLeaf(n4));
    EXPECT_TRUE(f1.isLeaf(n5));
    EXPECT_TRUE(f1.isLeaf(n6));
    EXPECT_TRUE(f1.isLeaf(n7));
    EXPECT_TRUE(f1.isLeaf(n8));

    EXPECT_TRUE(f1.isInTree(n1));
    EXPECT_TRUE(f1.isInTree(n2));
    EXPECT_TRUE(f1.isInTree(n3));
    EXPECT_TRUE(f1.isInTree(n4));
    EXPECT_TRUE(f1.isInTree(n5));
    EXPECT_TRUE(f1.isInTree(n6));
    EXPECT_TRUE(f1.isInTree(n7));
    EXPECT_TRUE(f1.isInTree(n8));
}

TEST_F(ForestTest, CycleTest)
{
    auto e1 = Edge<int, int>::createPtr(n1, n2, 1);
    auto e2 = Edge<int, int>::createPtr(n2, n3, 2);
    auto e3 = Edge<int, int>::createPtr(n3, n1, 3);

    auto f1 = Forest<int, int>({.nodes = {n1, n2, n3}, .edges = {e1, e2, e3}});

    EXPECT_TRUE(f1.hasBrokenEdges());

    auto brokenEdges = f1.brokenEdges();

    EXPECT_EQ(brokenEdges.size(), 1);
    EXPECT_EQ(brokenEdges[0]->data(), 3);
}
