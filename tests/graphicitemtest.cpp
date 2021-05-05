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

#include "source/Model/GraphicItem/graphicitem.h"

TEST(GraphicItem, Position)
{
    auto* item1 = new GraphicItem();
    auto* item2 = new GraphicItem();

    item1->setPos(QPointF(10, 10));
    item2->setPos(QPointF(20, 20));

    item2->setParentItem(item1);

    EXPECT_EQ(item1->pos(), QPointF(10, 10));
    EXPECT_EQ(item2->pos(), QPointF(20, 20));

    EXPECT_EQ(item2->scenePos(), QPointF(30, 30));
}
