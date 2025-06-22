// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include "source/CommonTools/GroupUuid.h"
#include "source/CommonTools/SafeUuid.h"
#include "source/CommonTools/TermUuid.h"

TEST(UuidsTest, Creation)
{
    EXPECT_FALSE(SafeUuid::from("").has_value());
    EXPECT_FALSE(SafeUuid::from("123").has_value());

    EXPECT_FALSE(GroupUuid::from("").has_value());
    EXPECT_FALSE(GroupUuid::from("123").has_value());

    EXPECT_FALSE(TermUuid::from("").has_value());
    EXPECT_FALSE(TermUuid::from("123").has_value());

    constexpr auto validUuid1 = "94810de3-51b8-469e-b316-00248ffa2a45";
    constexpr auto validUuid2 = "{94810de3-51b8-469e-b316-00248ffa2a45}";

    EXPECT_TRUE(SafeUuid::from(validUuid1).has_value());
    EXPECT_TRUE(SafeUuid::from(validUuid2).has_value());

    EXPECT_TRUE(GroupUuid::from(validUuid1).has_value());
    EXPECT_TRUE(GroupUuid::from(validUuid2).has_value());

    EXPECT_TRUE(TermUuid::from(validUuid1).has_value());
    EXPECT_TRUE(TermUuid::from(validUuid2).has_value());

    EXPECT_TRUE(SafeUuid::from(QUuid::createUuid()).has_value());
    EXPECT_TRUE(GroupUuid::from(QUuid::createUuid()).has_value());
    EXPECT_TRUE(TermUuid::from(QUuid::createUuid()).has_value());
}

TEST(UuidsTest, Generation)
{
    EXPECT_FALSE(SafeUuid::generate().get().isNull());
    EXPECT_FALSE(GroupUuid::generate().get().isNull());
    EXPECT_FALSE(TermUuid::generate().get().isNull());
}
