// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include "source/graph/Edge.hpp"

using namespace graph;

TEST(EdgeTest, InitEdge)
{
    auto n1 = Node<int>::createPtr(1);
    auto n2 = Node<int>::createPtr(2);

    auto edge = Edge<Node<int>, int>::createPtr(n1, n2);

    EXPECT_EQ(edge->root()->data(), 1);
    EXPECT_EQ(edge->leaf()->data(), 2);
}

TEST(EdgeTest, EdgeList)
{
    auto n1 = Node<int>::createPtr(1);
    auto n2 = Node<int>::createPtr(2);

    auto e1 = Edge<Node<int>, int>::createPtr(n1, n2);
    auto e2 = Edge<Node<int>, int>::createPtr(n2, n1);

    Edge<Node<int>, int>::List edgeList;

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

    auto edge = Edge<Node<int>, int>::createPtr(n1, n2, 15);

    EXPECT_EQ(edge->data(), 15);

    edge->setData(55 + 12);

    EXPECT_EQ(edge->data(), 67);
}

TEST(EdgeTest, Incidence)
{
    auto n1 = Node<int>::createPtr(1);
    auto n2 = Node<int>::createPtr(2);
    auto n3 = Node<int>::createPtr(3);

    auto e1 = Edge<Node<int>, int>::createPtr(n1, n2);
    auto e2 = Edge<Node<int>, int>::createPtr(n2, n1);

    EXPECT_TRUE(e1->incidentalTo(n1));
    EXPECT_TRUE(e1->incidentalTo(n2));

    EXPECT_TRUE(e2->incidentalTo(n1));
    EXPECT_TRUE(e2->incidentalTo(n2));

    EXPECT_FALSE(e1->incidentalTo(n3));
    EXPECT_FALSE(e2->incidentalTo(n3));
}

TEST(EdgeTest, Opposite)
{
    auto n1 = Node<int>::createPtr(1);
    auto n2 = Node<int>::createPtr(2);
    auto n3 = Node<int>::createPtr(3);

    auto e1 = Edge<Node<int>, int>::createPtr(n1, n2);
    auto e2 = Edge<Node<int>, int>::createPtr(n2, n3);

    // n1 - n2
    EXPECT_EQ(e1->oppositeTo(n1)->data(), 2);
    EXPECT_EQ(e1->oppositeTo(n2)->data(), 1);

    // n2 - n3
    EXPECT_EQ(e2->oppositeTo(n2)->data(), 3);
    EXPECT_EQ(e2->oppositeTo(n3)->data(), 2);
}
