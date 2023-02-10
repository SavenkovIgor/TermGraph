// Copyright © 2016-2022. Savenkov Igor
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

    auto allFiles = StaticDataStorage::files();
    QFileInfoList filteredFiles;

    QStringList filters;
    filters << "Global.json";

    for (const auto& fileInfo : allFiles) {
        if (filters.contains(fileInfo.fileName())) {
            filteredFiles << fileInfo;
        }
    }

    for (const auto& fileInfo : filteredFiles) {
        auto fileData = StaticDataStorage::qrcFileData(fileInfo.absoluteFilePath());
        auto group    = StaticGroupData::from(fileData);

        EXPECT_TRUE(group.has_value());

        auto castedJson = static_cast<QByteArray>(group.value());

        EXPECT_EQ(castedJson, fileData);

        if (castedJson != fileData) {
            qInfo() << "Raw file data:" << fileData;
            qInfo() << "Expected data:" << castedJson;
        }
    }
}
