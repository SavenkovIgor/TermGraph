// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include "source/model/graphicitem/graphicitem.h"

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
