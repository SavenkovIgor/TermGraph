// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include <QDebug>

#include <StaticDataStorage/StaticDataStorage.h>

TEST(StaticStorageTest, GroupsLoadable)
{
    auto storage = std::make_unique<StaticDataStorage>();

    auto group = storage->groups();

    EXPECT_TRUE(group.result().has_value());
}

TEST(StaticStorageTest, GroupsSerializeSymmetry)
{
    auto storage = std::make_unique<StaticDataStorage>();

    for (const auto& fileInfo : StaticDataStorage::files()) {
        auto fileData = StaticDataStorage::qrcFileData(fileInfo.absoluteFilePath());

        EXPECT_TRUE(fileData.isValidUtf8());

        auto group = StaticGroupData::from(fileData);

        EXPECT_TRUE(group.has_value());
        if (!group.has_value()) {
            qInfo() << "File path:" << fileInfo.absoluteFilePath();
        }

        auto castedJson = static_cast<QByteArray>(group.value());

        EXPECT_TRUE(castedJson.isValidUtf8());

        EXPECT_EQ(castedJson.size(), fileData.size());
        EXPECT_EQ(castedJson, fileData);

        if (castedJson != fileData) {
            qInfo() << "Raw file data:" << fileData;
            qInfo() << "Expected data:" << castedJson;
        }
    }
}
