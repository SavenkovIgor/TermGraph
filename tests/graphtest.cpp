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
#include "source/Model/Base/graph.hpp"
#include "source/Model/Base/node.hpp"

TEST(GraphTest, InitEdge)
{
    auto n1 = Node<int>::createPtr(1);
    auto n2 = Node<int>::createPtr(2);
    auto e1 = Edge<int, int>::createPtr(n1, n2, 0);

    auto data = GraphData<int, int>{.nodes = {n1, n2}, .edges = {e1}};

    auto list = GraphData<int, int>::List{{.nodes = {n1, n2}, .edges = {e1}}, {.nodes = {n2, n1}, .edges = {}}};

    Graph<int, int, int, int> graph{data};

    EXPECT_EQ(graph.nodeAt(0)->data(), 1);
    EXPECT_EQ(graph.nodeAt(1)->data(), 2);
    EXPECT_EQ(graph.edgeAt(0)->data(), 0);

    EXPECT_EQ(list[1].nodes[0]->data(), 2);
}
