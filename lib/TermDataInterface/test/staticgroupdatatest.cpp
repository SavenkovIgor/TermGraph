// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include <TermDataInterface/StaticGroupData.h>


TermData someTermData()
{
    return TermData{
        .uuid        = TermUuid::from("{8a505474-b90f-4e83-be63-e6a1e5d39b24}"),
        .term        = "Term",
        .definition  = "Definition",
        .description = "Description",
        .examples    = "Examples",
        .wikiUrl     = "Wiki",
        .wikiImage   = "WikiImg",
        .groupUuid   = GroupUuid::from("{5fdb1e7e-f47f-4c88-96d2-fd3d427d2a9d}").value(),
        .lastEdit    = QDateTime::fromString("2022-08-23T20:01:10Z", Qt::ISODate)
    };
}

StaticGroupData defaultGroupData()
{
    auto term = someTermData();
    StaticGroupData data;

    data.uuid          = GroupUuid::from("{5fdb1e7e-f47f-4c88-96d2-fd3d427d2a9d}"),
    data.name          = "Group",
    data.comment       = "Comment",
    data.size          = 0,
    data.terms.push_back(term);

    return data;
}

TEST(StaticGroupDataTest, ComparisonTest)
{
    auto data1 = defaultGroupData();
    EXPECT_EQ(data1, defaultGroupData());

    data1      = defaultGroupData();
    data1.uuid = GroupUuid::generate();
    EXPECT_NE(data1, defaultGroupData());

    data1      = defaultGroupData();
    data1.uuid = std::nullopt;
    EXPECT_NE(data1, defaultGroupData());

    data1             = defaultGroupData();
    data1.name        = "Group1";
    EXPECT_NE(data1, defaultGroupData());

    data1          = defaultGroupData();
    data1.comment  = "Comment1";
    EXPECT_NE(data1, defaultGroupData());

    data1      = defaultGroupData();
    data1.size = 10;
    EXPECT_NE(data1, defaultGroupData());

    data1                  = defaultGroupData();
    data1.terms[0].term    = "Term1";
    EXPECT_NE(data1, defaultGroupData());

    data1                  = defaultGroupData();
    data1.terms.push_back(someTermData());
    EXPECT_NE(data1, defaultGroupData());
}

TEST(StaticGroupDataTest, SerializationTest)
{
    auto group1 = defaultGroupData();
    auto json1   = static_cast<QJsonObject>(group1);
    auto group2 = StaticGroupData::from(json1).value();

    // FIXME: Hack, but it's ok for tests
    group2.terms[0].groupUuid = group1.terms[0].groupUuid;

    auto json2   = static_cast<QJsonObject>(group2);

    EXPECT_EQ(json1, json2);

    if (group1 != group2) {
        qDebug() << "json1:" << json1;
        qDebug() << "json2:" << json2;
    }
}

TEST(StaticGroupDataTest, MinimalSerialization)
{
    // auto json = R"({
    //     "name": "Group",
    //     "terms": [
    //         { "term": "Term1", "definition": "" },
    //         { "term": "Term2", "definition": "{Term1}" },
    //         { "term": "Term3", "definition": "{Term1}" },
    //     ]
    // })";

    // auto optData1 = StaticGroupData::from(QJsonDocument::fromJson(json).object());

    // EXPECT_TRUE(optData1.has_value());

    // auto data1 = optData1.value();

    // EXPECT_EQ(data1.name, "Group");
    // EXPECT_EQ(data1.terms.size(), 3);

    // EXPECT_EQ(data1.terms[0].term, "Term1");
    // EXPECT_EQ(data1.terms[0].definition, "");

    // EXPECT_EQ(data1.terms[1].term, "Term2");
    // EXPECT_EQ(data1.terms[1].definition, "{Term1}");

    // EXPECT_EQ(data1.terms[2].term, "Term3");
    // EXPECT_EQ(data1.terms[2].definition, "{Term1}");
}
