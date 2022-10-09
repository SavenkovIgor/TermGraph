// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include <TermDataInterface/GroupData.h>

GroupData defaultData() {
    return GroupData{
        .uuid          = GroupUuid::create("{8a505474-b90f-4e83-be63-e6a1e5d39b24}"),
        .name          = "Group",
        .comment       = "Comment",
        .size          = 1,
        .lastEdit      = QDateTime::currentDateTime(),
        .nodesLastEdit = QDateTime::currentDateTime()
    };
}

TEST(GroupDataTest, ComparisonTest)
{
    auto data1 = defaultData();
    EXPECT_EQ(data1, defaultData());

    data1 = defaultData();
    data1.uuid = GroupUuid::generate();
    EXPECT_NE(data1, defaultData());

    data1 = defaultData();
    data1.uuid = std::nullopt;
    EXPECT_NE(data1, defaultData());

    data1 = defaultData();
    data1.name        = "Group1";
    EXPECT_NE(data1, defaultData());

    data1 = defaultData();
    data1.comment  = "Comment1";
    EXPECT_NE(data1, defaultData());

    data1 = defaultData();
    data1.size = 10;
    EXPECT_NE(data1, defaultData());

    data1 = defaultData();
    data1.lastEdit    = QDateTime::currentDateTime().addSecs(1);
    EXPECT_NE(data1, defaultData());

    data1 = defaultData();
    data1.nodesLastEdit    = QDateTime::currentDateTime().addSecs(1);
    EXPECT_NE(data1, defaultData());

    data1 = defaultData();
    data1.nodesLastEdit    = std::nullopt;
    EXPECT_NE(data1, defaultData());
}
