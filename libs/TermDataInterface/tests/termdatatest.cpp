// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include <TermDataInterface/TermData.h>

TermData defaultData() {
    return TermData{
        .uuid        = TermUuid::create("{8a505474-b90f-4e83-be63-e6a1e5d39b24}"),
        .term        = "Term",
        .definition  = "Definition",
        .description = "Description",
        .examples    = "Examples",
        .wikiUrl     = "Wiki",
        .wikiImage   = "WikiImg",
        .groupUuid   = GroupUuid::create("{5fdb1e7e-f47f-4c88-96d2-fd3d427d2a9d}").value(),
        .lastEdit    = QDateTime::currentDateTime()
    };
}

TEST(TermDataTest, ComparisonTest)
{
    auto data1 = defaultData();
    EXPECT_EQ(data1, defaultData());

    data1 = defaultData();
    data1.uuid = TermUuid::generate();
    EXPECT_NE(data1, defaultData());

    data1 = defaultData();
    data1.term        = "Term1";
    EXPECT_NE(data1, defaultData());

    data1 = defaultData();
    data1.definition  = "Definition1";
    EXPECT_NE(data1, defaultData());

    data1 = defaultData();
    data1.description = "Description1";
    EXPECT_NE(data1, defaultData());

    data1 = defaultData();
    data1.examples    = "Examples1";
    EXPECT_NE(data1, defaultData());

    data1 = defaultData();
    data1.wikiUrl     = "Wiki1";
    EXPECT_NE(data1, defaultData());

    data1 = defaultData();
    data1.wikiImage   = "WikiImg1";
    EXPECT_NE(data1, defaultData());

    data1 = defaultData();
    data1.groupUuid   = GroupUuid::generate();
    EXPECT_NE(data1, defaultData());

    data1 = defaultData();
    data1.lastEdit    = QDateTime::currentDateTime().addSecs(1);
    EXPECT_NE(data1, defaultData());
}
