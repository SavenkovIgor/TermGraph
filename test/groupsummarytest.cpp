// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include "source/TermDataInterface/GroupSummary.h"

GroupSummary defaultGroupSummary()
{
    return GroupSummary{.uuid          = GroupUuid::from("{8a505474-b90f-4e83-be63-e6a1e5d39b24}"),
                        .name          = "Group",
                        .comment       = "Comment",
                        .size          = 1,
                        .lastEdit      = QDateTime::fromString("2022-08-23T20:01:10Z", Qt::ISODate),
                        .nodesLastEdit = QDateTime::fromString("2022-08-23T20:01:10Z", Qt::ISODate)};
}

TEST(GroupSummaryTest, ComparisonTest)
{
    auto data1 = defaultGroupSummary();
    EXPECT_EQ(data1, defaultGroupSummary());

    data1      = defaultGroupSummary();
    data1.uuid = GroupUuid::generate();
    EXPECT_NE(data1, defaultGroupSummary());

    data1      = defaultGroupSummary();
    data1.uuid = std::nullopt;
    EXPECT_NE(data1, defaultGroupSummary());

    data1      = defaultGroupSummary();
    data1.name = "Group1";
    EXPECT_NE(data1, defaultGroupSummary());

    data1         = defaultGroupSummary();
    data1.comment = "Comment1";
    EXPECT_NE(data1, defaultGroupSummary());

    data1      = defaultGroupSummary();
    data1.size = 10;
    EXPECT_NE(data1, defaultGroupSummary());

    data1          = defaultGroupSummary();
    data1.lastEdit = QDateTime::currentDateTime().addSecs(1);
    EXPECT_NE(data1, defaultGroupSummary());

    data1               = defaultGroupSummary();
    data1.nodesLastEdit = QDateTime::currentDateTime().addSecs(1);
    EXPECT_NE(data1, defaultGroupSummary());

    data1               = defaultGroupSummary();
    data1.nodesLastEdit = std::nullopt;
    EXPECT_NE(data1, defaultGroupSummary());
}
