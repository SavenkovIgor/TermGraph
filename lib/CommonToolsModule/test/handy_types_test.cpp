// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

import CommonTools.HandyTypes;

TEST(HandyTypesTest, Result)
{
    Result<int> r = 1;
    EXPECT_TRUE(r);
    EXPECT_TRUE(r.has_value());
    EXPECT_EQ(r.value(), 1);

    Result<int> r2 = ErrorCodes::TermEmpty;
    EXPECT_FALSE(r2);
    EXPECT_FALSE(r2.has_value());
    EXPECT_EQ(r2.error(), ErrorCodes::TermEmpty);
}
