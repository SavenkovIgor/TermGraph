// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include <CommonTools/GroupUuid.h>
#include <CommonTools/SafeUuid.h>
#include <CommonTools/TermUuid.h>

TEST(UuidsTest, Creation)
{
    EXPECT_FALSE(SafeUuid::create("").has_value());
    EXPECT_FALSE(SafeUuid::create("123").has_value());

    EXPECT_FALSE(GroupUuid::create("").has_value());
    EXPECT_FALSE(GroupUuid::create("123").has_value());

    EXPECT_FALSE(TermUuid::create("").has_value());
    EXPECT_FALSE(TermUuid::create("123").has_value());

    constexpr auto validUuid1 = "94810de3-51b8-469e-b316-00248ffa2a45";
    constexpr auto validUuid2 = "{94810de3-51b8-469e-b316-00248ffa2a45}";

    EXPECT_TRUE(SafeUuid::create(validUuid1).has_value());
    EXPECT_TRUE(SafeUuid::create(validUuid2).has_value());

    EXPECT_TRUE(GroupUuid::create(validUuid1).has_value());
    EXPECT_TRUE(GroupUuid::create(validUuid2).has_value());

    EXPECT_TRUE(TermUuid::create(validUuid1).has_value());
    EXPECT_TRUE(TermUuid::create(validUuid2).has_value());

    EXPECT_TRUE(SafeUuid::create(QUuid::createUuid()).has_value());
    EXPECT_TRUE(GroupUuid::create(QUuid::createUuid()).has_value());
    EXPECT_TRUE(TermUuid::create(QUuid::createUuid()).has_value());
}

TEST(UuidsTest, Generation)
{
    EXPECT_FALSE(SafeUuid::generate().get().isNull());
    EXPECT_FALSE(GroupUuid::generate().get().isNull());
    EXPECT_FALSE(TermUuid::generate().get().isNull());
}
