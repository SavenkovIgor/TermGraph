// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

// #include <memory>

// #include <QDir>

#include <gtest/gtest.h>

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
        auto group    = GroupData::from(fileData);

        EXPECT_TRUE(group.has_value());

        auto groupData = static_cast<QByteArray>(group.value());

        EXPECT_EQ(groupData, fileData);
    }
}
