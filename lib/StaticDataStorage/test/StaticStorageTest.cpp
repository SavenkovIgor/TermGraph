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
