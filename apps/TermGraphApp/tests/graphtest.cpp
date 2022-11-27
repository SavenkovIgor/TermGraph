// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include <QSet>

#include "source/model/base/graph.hpp"
#include "source/model/base/graphdata.hpp"

class GraphTest : public ::testing::Test
{
public:
    using NodeT      = Node<int>;
    using EdgeT      = Edge<NodeT, int>;
    using GraphT     = Graph<NodeT, EdgeT>;
    using GraphDataT = GraphData<NodeT, EdgeT>;

    static const NodeT::Ptr n1;
    static const NodeT::Ptr n2;
    static const NodeT::Ptr n3;
    static const NodeT::Ptr n4;
    static const NodeT::Ptr n5;

    static const EdgeT::Ptr e1;
    static const EdgeT::Ptr e2;
};

const GraphTest::NodeT::Ptr GraphTest::n1 = NodeT::createPtr(1);
const GraphTest::NodeT::Ptr GraphTest::n2 = NodeT::createPtr(2);
const GraphTest::NodeT::Ptr GraphTest::n3 = NodeT::createPtr(3);
const GraphTest::NodeT::Ptr GraphTest::n4 = NodeT::createPtr(4);
const GraphTest::NodeT::Ptr GraphTest::n5 = NodeT::createPtr(5);

const GraphTest::EdgeT::Ptr GraphTest::e1 = EdgeT::createPtr(n1, n1, 1);
const GraphTest::EdgeT::Ptr GraphTest::e2 = EdgeT::createPtr(n3, n5, 2);

TEST_F(GraphTest, GraphDataInit)
{
    auto list = GraphDataT::List{{.nodes = {n1, n2}, .edges = {e1}}, {.nodes = {n2, n1}, .edges = {}}};

    EXPECT_EQ(list[0].edges.size(), 1);
    EXPECT_EQ(list[1].edges.size(), 0);

    auto graph = GraphT({.nodes = {n1, n2, n3}, .edges = {e1, e2}});

    EXPECT_EQ(graph.nodeList().size(), 3);
    EXPECT_EQ(graph.edgeList().size(), 2);
}

TEST_F(GraphTest, GraphDataContains)
{
    auto data = GraphDataT{.nodes = {n1, n2}, .edges = {e1}};

    EXPECT_TRUE(data.contains(n1));
    EXPECT_TRUE(data.contains(n2));
    EXPECT_FALSE(data.contains(n3));

    EXPECT_TRUE(data.contains(e1));
    EXPECT_FALSE(data.contains(e2));
}

TEST_F(GraphTest, GraphDataFilter)
{
    auto data = GraphDataT{.nodes = {n1, n2, n3, n4}, .edges = {e1, e2}};

    auto fNodes = data.filterNodes([](auto node) { return node->data() % 2 == 0; });

    EXPECT_EQ(fNodes.size(), 2);
    EXPECT_EQ(fNodes[0]->data(), 2);
    EXPECT_EQ(fNodes[1]->data(), 4);

    auto fEdges = data.filterEdges([](auto edge) { return edge->data() % 2 == 1; });

    EXPECT_EQ(fEdges.size(), 1);
    EXPECT_EQ(fEdges[0]->data(), 1);
}

TEST_F(GraphTest, GraphDataSubtract)
{
    auto nodes1 = GraphDataT::subtractNodeList({n1, n2, n3, n4}, {n1, n3});
    EXPECT_EQ(nodes1.size(), 2);
    EXPECT_EQ(nodes1[0]->data(), 2);
    EXPECT_EQ(nodes1[1]->data(), 4);

    auto nodes2 = GraphDataT::subtractNodeList({n1, n2, n3, n4}, {n1, n2, n3, n4});
    EXPECT_EQ(nodes2.size(), 0);

    auto edges1 = GraphDataT::subtractEdgeList({e1, e2}, {});
    EXPECT_EQ(edges1.size(), 2);
    EXPECT_EQ(edges1[0]->data(), 1);
    EXPECT_EQ(edges1[1]->data(), 2);

    auto edges2 = GraphDataT::subtractEdgeList({e1, e2}, {e1});
    EXPECT_EQ(edges2.size(), 1);
    EXPECT_EQ(edges2[0]->data(), 2);
}

TEST_F(GraphTest, Contains)
{
    auto graph = GraphT({.nodes = {n1, n2, n3}, .edges = {e1, e2}});

    EXPECT_TRUE(graph.contains(n1));
    EXPECT_TRUE(graph.contains(n2));
    EXPECT_TRUE(graph.contains(n3));

    EXPECT_FALSE(graph.contains(n4));
    EXPECT_FALSE(graph.contains(n5));

    EXPECT_TRUE(graph.contains(e1));
    EXPECT_TRUE(graph.contains(e2));

    EXPECT_FALSE(graph.contains(EdgeT::createPtr(n1, n1, 3)));
}

TEST_F(GraphTest, Isolated)
{
    auto graph = GraphT({.nodes = {n1, n2, n3, n4, n5}, .edges = {e1, e2}});

    auto isolated = graph.isolatedNodes();

    EXPECT_EQ(isolated.size(), 2);
    EXPECT_EQ(isolated[0]->data(), 2);
    EXPECT_EQ(isolated[1]->data(), 4);
}

TEST_F(GraphTest, NotIsolated)
{
    auto graph = GraphT({.nodes = {n1, n2, n3, n4, n5}, .edges = {e1, e2}});

    auto connectedNodes = graph.connectedNodes();

    EXPECT_EQ(connectedNodes.size(), 3);
    EXPECT_EQ(connectedNodes[0]->data(), 1);
    EXPECT_EQ(connectedNodes[1]->data(), 3);
    EXPECT_EQ(connectedNodes[2]->data(), 5);
}

TEST_F(GraphTest, ConnectedEdges)
{
    auto e3 = EdgeT::createPtr(n3, n4, 3);

    auto graph = GraphT({.nodes = {n1, n2, n3, n4, n5}, .edges = {e1, e2, e3}});

    auto edges1 = graph.connectedEdges(n1);

    EXPECT_EQ(edges1.size(), 1);
    EXPECT_EQ(edges1[0]->data(), 1);

    auto edges2 = graph.connectedEdges(n2);
    EXPECT_EQ(edges2.size(), 0);

    auto edges3 = graph.connectedEdges(n3);
    EXPECT_EQ(edges3.size(), 2);
    EXPECT_EQ(edges3[0]->data(), 2);
    EXPECT_EQ(edges3[1]->data(), 3);
}

TEST_F(GraphTest, Surrounding)
{
    auto graph = GraphT({.nodes = {n1, n2, n3, n4, n5}, .edges = {e1, e2}});

    auto n1surr = graph.surrounding(n1);

    EXPECT_EQ(n1surr.nodes.size(), 1);
    EXPECT_EQ(n1surr.nodes[0]->data(), 1);
    EXPECT_EQ(n1surr.edges.size(), 1);
    EXPECT_EQ(n1surr.edges[0]->data(), 1);

    auto n2surr = graph.surrounding(n2);

    EXPECT_EQ(n2surr.nodes.size(), 0);
    EXPECT_EQ(n2surr.edges.size(), 0);

    auto n3surr = graph.surrounding(n3);

    EXPECT_EQ(n3surr.nodes.size(), 1);
    EXPECT_EQ(n3surr.nodes[0]->data(), 5);
    EXPECT_EQ(n3surr.edges.size(), 1);
    EXPECT_EQ(n3surr.edges[0]->data(), 2);

    auto n5surr = graph.surrounding(n5);

    EXPECT_EQ(n5surr.nodes.size(), 1);
    EXPECT_EQ(n5surr.nodes[0]->data(), 3);
    EXPECT_EQ(n5surr.edges.size(), 1);
    EXPECT_EQ(n5surr.edges[0]->data(), 2);
}

TEST_F(GraphTest, BondedSubgraphs)
{
    auto e3    = EdgeT::createPtr(n5, n4, 3);
    auto graph = GraphT({.nodes = {n1, n2, n3, n4, n5}, .edges = {e1, e2, e3}});

    auto subraphs = graph.bondedSubgraphs();

    EXPECT_EQ(subraphs.size(), 2);

    EXPECT_EQ(subraphs[0].nodes.size(), 1);
    EXPECT_EQ(subraphs[0].nodes[0]->data(), 1);
    EXPECT_EQ(subraphs[0].edges.size(), 1);
    EXPECT_EQ(subraphs[0].edges[0]->data(), 1);

    EXPECT_EQ(subraphs[1].nodes.size(), 3);

    QSet<int> nodeSet{3, 4, 5};
    for (const auto& node : subraphs[1].nodes)
        EXPECT_TRUE(nodeSet.remove(node->data()));

    EXPECT_TRUE(nodeSet.empty());

    QSet<int> edgeSet{2, 3};
    for (const auto& edge : subraphs[1].edges)
        EXPECT_TRUE(edgeSet.remove(edge->data()));

    EXPECT_TRUE(edgeSet.empty());
}
