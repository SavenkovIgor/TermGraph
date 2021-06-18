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

#include <set>

#include <gtest/gtest.h>

#include "source/Model/Base/forest.hpp"
#include "source/Model/Base/graph.hpp"
#include "source/Model/Base/graphdata.hpp"

class ForestTest : public ::testing::Test
{
public:
    using NodeT      = Node<int>;
    using EdgeT      = Edge<NodeT, int>;
    using GraphDataT = GraphData<NodeT, EdgeT>;
    using ForestT    = Forest<NodeT, EdgeT>;

    static const NodeT::Ptr n1;
    static const NodeT::Ptr n2;
    static const NodeT::Ptr n3;
    static const NodeT::Ptr n4;
    static const NodeT::Ptr n5;
    static const NodeT::Ptr n6;
    static const NodeT::Ptr n7;
    static const NodeT::Ptr n8;

    static const EdgeT::Ptr e1;
    static const EdgeT::Ptr e2;
    static const EdgeT::Ptr e3;
    static const EdgeT::Ptr e4;
    static const EdgeT::Ptr e5;
    static const EdgeT::Ptr e6;
    static const EdgeT::Ptr e7;
    static const EdgeT::Ptr e8;
    static const EdgeT::Ptr e9;
    static const EdgeT::Ptr e10;

    static const ForestT fullForest;
};

const ForestTest::NodeT::Ptr ForestTest::n1 = NodeT::createPtr(1);
const ForestTest::NodeT::Ptr ForestTest::n2 = NodeT::createPtr(2);
const ForestTest::NodeT::Ptr ForestTest::n3 = NodeT::createPtr(3);
const ForestTest::NodeT::Ptr ForestTest::n4 = NodeT::createPtr(4);
const ForestTest::NodeT::Ptr ForestTest::n5 = NodeT::createPtr(5);
const ForestTest::NodeT::Ptr ForestTest::n6 = NodeT::createPtr(6);
const ForestTest::NodeT::Ptr ForestTest::n7 = NodeT::createPtr(7);
const ForestTest::NodeT::Ptr ForestTest::n8 = NodeT::createPtr(8);

const ForestTest::EdgeT::Ptr ForestTest::e1 = EdgeT::createPtr(n1, n3, 1);
const ForestTest::EdgeT::Ptr ForestTest::e2 = EdgeT::createPtr(n1, n4, 2);
const ForestTest::EdgeT::Ptr ForestTest::e3 = EdgeT::createPtr(n1, n5, 3);
const ForestTest::EdgeT::Ptr ForestTest::e4 = EdgeT::createPtr(n2, n3, 4);
const ForestTest::EdgeT::Ptr ForestTest::e5 = EdgeT::createPtr(n2, n4, 5);
const ForestTest::EdgeT::Ptr ForestTest::e6 = EdgeT::createPtr(n5, n6, 6);
const ForestTest::EdgeT::Ptr ForestTest::e7 = EdgeT::createPtr(n5, n7, 7);
const ForestTest::EdgeT::Ptr ForestTest::e8 = EdgeT::createPtr(n6, n8, 8);
const ForestTest::EdgeT::Ptr ForestTest::e9 = EdgeT::createPtr(n7, n8, 9);

const ForestTest::ForestT ForestTest::fullForest = ForestTest::ForestT(
    {.nodes = {n1, n2, n3, n4, n5, n6, n7, n8}, .edges = {e1, e2, e3, e4, e5, e6, e7, e8, e9}});

TEST_F(ForestTest, NodeTypes)
{
    EXPECT_EQ(fullForest.nodeType(n1), NodeType::root);
    EXPECT_EQ(fullForest.nodeType(n2), NodeType::root);
    EXPECT_EQ(fullForest.nodeType(n3), NodeType::endLeaf);
    EXPECT_EQ(fullForest.nodeType(n4), NodeType::endLeaf);
    EXPECT_EQ(fullForest.nodeType(n5), NodeType::middleLeaf);
    EXPECT_EQ(fullForest.nodeType(n6), NodeType::middleLeaf);
    EXPECT_EQ(fullForest.nodeType(n7), NodeType::middleLeaf);
    EXPECT_EQ(fullForest.nodeType(n8), NodeType::endLeaf);
}

TEST_F(ForestTest, NodeTypes2)
{
    EXPECT_TRUE(fullForest.isRoot(n1));
    EXPECT_TRUE(fullForest.isRoot(n2));
    EXPECT_TRUE(fullForest.isLeaf(n3));
    EXPECT_TRUE(fullForest.isLeaf(n4));
    EXPECT_TRUE(fullForest.isLeaf(n5));
    EXPECT_TRUE(fullForest.isLeaf(n6));
    EXPECT_TRUE(fullForest.isLeaf(n7));
    EXPECT_TRUE(fullForest.isLeaf(n8));
}

TEST_F(ForestTest, CycleTest)
{
    auto e1 = EdgeT::createPtr(n1, n2, 1);
    auto e2 = EdgeT::createPtr(n2, n3, 2);
    auto e3 = EdgeT::createPtr(n3, n1, 3);

    auto f1 = ForestT({.nodes = {n1, n2, n3}, .edges = {e1, e2, e3}});

    EXPECT_TRUE(f1.hasBrokenEdges());

    auto brokenEdges = f1.brokenEdges();

    EXPECT_EQ(brokenEdges.size(), 1);
    EXPECT_EQ(brokenEdges[0]->data(), 3);
}

TEST_F(ForestTest, RootVisit)
{
    std::set<NodeT::Ptr> visitList;

    auto visitListInserter = [&visitList](auto node) {
        visitList.insert(node);
        return false;
    };

    fullForest.rootsVisiter(n1, visitListInserter);
    EXPECT_TRUE(visitList.empty());
    visitList.clear();

    fullForest.rootsVisiter(n2, visitListInserter);
    EXPECT_TRUE(visitList.empty());
    visitList.clear();

    fullForest.rootsVisiter(n3, visitListInserter);
    EXPECT_TRUE(visitList.size() == 2);
    EXPECT_TRUE(visitList.contains(n1));
    EXPECT_TRUE(visitList.contains(n2));
    visitList.clear();

    fullForest.rootsVisiter(n8, visitListInserter);
    EXPECT_TRUE(visitList.size() == 4);
    EXPECT_TRUE(visitList.contains(n1));
    EXPECT_TRUE(visitList.contains(n5));
    EXPECT_TRUE(visitList.contains(n6));
    EXPECT_TRUE(visitList.contains(n7));
    visitList.clear();
}

TEST_F(ForestTest, LeafVisit)
{
    std::set<NodeT::Ptr> visitList;

    auto visitListInserter = [&visitList](auto node) {
        visitList.insert(node);
        return false;
    };

    fullForest.leafsVisiter(n8, visitListInserter);
    EXPECT_TRUE(visitList.empty());
    visitList.clear();

    fullForest.leafsVisiter(n3, visitListInserter);
    EXPECT_TRUE(visitList.empty());
    visitList.clear();

    fullForest.leafsVisiter(n2, visitListInserter);
    EXPECT_TRUE(visitList.size() == 2);
    EXPECT_TRUE(visitList.contains(n3));
    EXPECT_TRUE(visitList.contains(n4));
    visitList.clear();

    fullForest.leafsVisiter(n1, visitListInserter);
    EXPECT_TRUE(visitList.size() == 6);
    EXPECT_TRUE(visitList.contains(n3));
    EXPECT_TRUE(visitList.contains(n4));
    EXPECT_TRUE(visitList.contains(n5));
    EXPECT_TRUE(visitList.contains(n6));
    EXPECT_TRUE(visitList.contains(n7));
    EXPECT_TRUE(visitList.contains(n8));
    visitList.clear();
}

TEST_F(ForestTest, RootNodes)
{
    auto rNodes1 = fullForest.rootNodes(n5);

    EXPECT_EQ(rNodes1.size(), 1);
    EXPECT_EQ(rNodes1[0]->data(), 1);

    auto rNodes2 = fullForest.rootNodes(n8);

    EXPECT_EQ(rNodes2.size(), 2);
    EXPECT_EQ(rNodes2[0]->data(), 6);
    EXPECT_EQ(rNodes2[1]->data(), 7);
}

TEST_F(ForestTest, LeafNodes)
{
    auto lNodes1 = fullForest.leafNodes(n5);

    EXPECT_EQ(lNodes1.size(), 2);
    EXPECT_EQ(lNodes1[0]->data(), 6);
    EXPECT_EQ(lNodes1[1]->data(), 7);

    auto lNodes2 = fullForest.leafNodes(n1);

    EXPECT_EQ(lNodes2.size(), 3);
    EXPECT_EQ(lNodes2[0]->data(), 3);
    EXPECT_EQ(lNodes2[1]->data(), 4);
    EXPECT_EQ(lNodes2[2]->data(), 5);
}

TEST_F(ForestTest, HasAncestor)
{
    EXPECT_TRUE(fullForest.isAncestor(n6, n1));
    EXPECT_TRUE(fullForest.isAncestor(n4, n1));
    EXPECT_TRUE(fullForest.isAncestor(n8, n5));
    EXPECT_TRUE(fullForest.isAncestor(n8, n7));

    EXPECT_FALSE(fullForest.isAncestor(n8, n3));
    EXPECT_FALSE(fullForest.isAncestor(n4, n8));
}

TEST_F(ForestTest, hasFarAncestor)
{
    EXPECT_TRUE(fullForest.isFarAncestor(n8, n5));
    EXPECT_TRUE(fullForest.isFarAncestor(n8, n1));

    EXPECT_FALSE(fullForest.isFarAncestor(n8, n7));
    EXPECT_FALSE(fullForest.isFarAncestor(n8, n6));
}

TEST_F(ForestTest, WasteEdges)
{
    const auto e10 = EdgeT::createPtr(n5, n8, 10);
    const auto e11 = EdgeT::createPtr(n1, n8, 11);
    const auto e12 = EdgeT::createPtr(n1, n6, 12);
    const auto e13 = EdgeT::createPtr(n1, n7, 13);

    const ForestT forest = ForestT(
        {.nodes = {n1, n2, n3, n4, n5, n6, n7, n8}, .edges = {e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13}});

    EXPECT_FALSE(forest.contains(e10));
    EXPECT_FALSE(forest.contains(e11));
    EXPECT_FALSE(forest.contains(e12));
    EXPECT_FALSE(forest.contains(e13));

    EXPECT_TRUE(forest.hasWasteEdges());
    EXPECT_EQ(forest.wasteEdges().size(), 4);
    EXPECT_EQ(forest.wasteEdges()[0]->data(), 13);
    EXPECT_EQ(forest.wasteEdges()[1]->data(), 11);
    EXPECT_EQ(forest.wasteEdges()[2]->data(), 12);
    EXPECT_EQ(forest.wasteEdges()[3]->data(), 10);
}

TEST_F(ForestTest, Levels)
{
    const auto e10 = EdgeT::createPtr(n5, n8, 10);
    const auto e11 = EdgeT::createPtr(n1, n8, 11);
    const auto e12 = EdgeT::createPtr(n1, n6, 12);
    const auto e13 = EdgeT::createPtr(n1, n7, 13);

    const ForestT forest = ForestT(
        {.nodes = {n1, n2, n3, n4, n5, n6, n7, n8}, .edges = {e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13}});

    EXPECT_EQ(fullForest.level(n1), 0);
    EXPECT_EQ(fullForest.level(n2), 0);
    EXPECT_EQ(fullForest.level(n3), 1);
    EXPECT_EQ(fullForest.level(n4), 1);
    EXPECT_EQ(fullForest.level(n5), 1);
    EXPECT_EQ(fullForest.level(n6), 2);
    EXPECT_EQ(fullForest.level(n7), 2);
    EXPECT_EQ(fullForest.level(n8), 3);

    auto level0 = fullForest.nodesAtLevel(0);

    EXPECT_EQ(level0.size(), 2);
    EXPECT_EQ(level0[0]->data(), 1);
    EXPECT_EQ(level0[1]->data(), 2);

    auto level1 = fullForest.nodesAtLevel(1);

    EXPECT_EQ(level1.size(), 3);
    EXPECT_EQ(level1[0]->data(), 3);
    EXPECT_EQ(level1[1]->data(), 4);
    EXPECT_EQ(level1[2]->data(), 5);

    auto level2 = fullForest.nodesAtLevel(2);

    EXPECT_EQ(level2.size(), 2);
    EXPECT_EQ(level2[0]->data(), 6);
    EXPECT_EQ(level2[1]->data(), 7);

    auto level3 = fullForest.nodesAtLevel(3);

    EXPECT_EQ(level3.size(), 1);
    EXPECT_EQ(level3[0]->data(), 8);
}
