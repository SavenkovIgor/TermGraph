// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include <TermDataInterface/GroupData.h>


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

GroupData defaultGroupData()
{
    auto term = someTermData();
    auto data = GroupData();

    data.uuid          = GroupUuid::from("{5fdb1e7e-f47f-4c88-96d2-fd3d427d2a9d}"),
    data.name          = "Group",
    data.comment       = "Comment",
    data.size          = 0,
    data.lastEdit      = QDateTime::fromString("2022-08-23T20:01:10Z", Qt::ISODate),
    data.nodesLastEdit = QDateTime::fromString("2022-08-23T20:01:10Z", Qt::ISODate),
    data.terms.push_back(term);

    return data;
}

TEST(GroupDataTest, ComparisonTest)
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

    data1             = defaultGroupData();
    data1.lastEdit    = QDateTime::currentDateTime().addSecs(1);
    EXPECT_NE(data1, defaultGroupData());

    data1                  = defaultGroupData();
    data1.nodesLastEdit    = QDateTime::currentDateTime().addSecs(1);
    EXPECT_NE(data1, defaultGroupData());

    data1                  = defaultGroupData();
    data1.nodesLastEdit    = std::nullopt;
    EXPECT_NE(data1, defaultGroupData());

    data1                  = defaultGroupData();
    data1.terms[0].term    = "Term1";
    EXPECT_NE(data1, defaultGroupData());

    data1                  = defaultGroupData();
    data1.terms.push_back(someTermData());
    EXPECT_NE(data1, defaultGroupData());
}

TEST(GroupDataTest, SerializationTest)
{
    auto data1 = defaultGroupData();
    auto data2 = GroupData::from(static_cast<QJsonObject>(data1));


    qInfo() << static_cast<QJsonObject>(data1);

    EXPECT_EQ(data1, data2);

    data1 = defaultGroupData();
    data2 = GroupData::from(static_cast<QByteArray>(data1));

    EXPECT_EQ(data1, data2);
}

TEST(GroupDataTest, MinimalSerialization)
{
    // auto json = R"({
    //     "name": "Group",
    //     "terms": [
    //         { "term": "Term1", "definition": "" },
    //         { "term": "Term2", "definition": "{Term1}" },
    //         { "term": "Term3", "definition": "{Term1}" },
    //     ]
    // })";

    // auto optData1 = GroupData::from(QJsonDocument::fromJson(json).object());

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
