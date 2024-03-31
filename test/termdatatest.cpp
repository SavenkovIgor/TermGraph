// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include <source/TermDataInterface/TermData.h>

TermData defaultTermData()
{
    return TermData{.uuid          = TermUuid::from("{8a505474-b90f-4e83-be63-e6a1e5d39b24}"),
                    .term          = "Term",
                    .definition    = "Definition",
                    .description   = "Description",
                    .examples      = "Examples",
                    .wikiUrl       = "Wiki",
                    .wikiImage     = "WikiImg",
                    .knowledgeArea = "",
                    .groupUuid     = GroupUuid::from("{5fdb1e7e-f47f-4c88-96d2-fd3d427d2a9d}").value(),
                    .lastEdit      = QDateTime::fromString("2022-08-23T20:01:10Z", Qt::ISODate)};
}

TermData::List defaultTermDataList()
{
    auto data1 = defaultTermData();
    auto data2 = defaultTermData();
    auto data3 = defaultTermData();

    auto ret = TermData::List();

    ret.push_back(data1);
    ret.push_back(data2);
    ret.push_back(data3);

    return ret;
}

TEST(TermDataTest, ComparisonTest)
{
    auto data1 = defaultTermData();
    EXPECT_EQ(data1, defaultTermData());

    data1      = defaultTermData();
    data1.uuid = TermUuid::generate();
    EXPECT_NE(data1, defaultTermData());

    data1      = defaultTermData();
    data1.uuid = std::nullopt;
    EXPECT_NE(data1, defaultTermData());

    data1      = defaultTermData();
    data1.term = "Term1";
    EXPECT_NE(data1, defaultTermData());

    data1            = defaultTermData();
    data1.definition = "Definition1";
    EXPECT_NE(data1, defaultTermData());

    data1             = defaultTermData();
    data1.description = "Description1";
    EXPECT_NE(data1, defaultTermData());

    data1          = defaultTermData();
    data1.examples = "Examples1";
    EXPECT_NE(data1, defaultTermData());

    data1         = defaultTermData();
    data1.wikiUrl = "Wiki1";
    EXPECT_NE(data1, defaultTermData());

    data1           = defaultTermData();
    data1.wikiImage = "WikiImg1";
    EXPECT_NE(data1, defaultTermData());

    data1           = defaultTermData();
    data1.groupUuid = GroupUuid::generate();
    EXPECT_NE(data1, defaultTermData());

    data1          = defaultTermData();
    data1.lastEdit = QDateTime::currentDateTime().addSecs(1);
    EXPECT_NE(data1, defaultTermData());
}

TEST(TermDataTest, IsNullTest)
{
    auto data1 = defaultTermData();
    EXPECT_FALSE(data1.isNull());

    data1      = defaultTermData();
    data1.term = "";
    EXPECT_TRUE(data1.isNull());
}

TEST(TermDataTest, JsonTest)
{
    auto data1 = defaultTermData();
    auto json  = static_cast<QJsonObject>(data1);
    auto data2 = TermData::from(json, TermData::JsonCheckMode::Import);

    EXPECT_TRUE(data2.has_value());
    EXPECT_EQ(data1, data2.value());
}

TEST(TermDataTest, JsonListTest)
{
    auto data1     = defaultTermDataList();
    auto jsonArray = static_cast<QJsonArray>(data1);
    auto data2     = TermData::List::from(jsonArray);

    EXPECT_EQ(data1.size(), data2.size());
    EXPECT_EQ(data1, data2);
}

TEST(TermDataTest, JsonListEmptyTest)
{
    auto data1     = TermData::List{};
    auto jsonArray = static_cast<QJsonArray>(data1);
    auto data2     = TermData::List::from(jsonArray);

    EXPECT_EQ(data1.size(), 0);
    EXPECT_EQ(data1.size(), data2.size());
    EXPECT_EQ(data1, data2);
    EXPECT_TRUE(data2.empty());
}

TEST(TermDataTest, JsonListInvalidTest)
{
    auto jsonArray = QJsonArray{};
    auto data2     = TermData::List::from(jsonArray);

    EXPECT_TRUE(data2.empty());
}

TEST(TermDataTest, JsonListObjectTest)
{
    auto data1 = defaultTermDataList();

    auto jsonObj = static_cast<QJsonObject>(data1);
    auto data2   = TermData::List::from(jsonObj);

    EXPECT_TRUE(data2.has_value());
    EXPECT_EQ(data1, data2.value());
}
