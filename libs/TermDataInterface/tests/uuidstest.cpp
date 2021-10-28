/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

#include <gtest/gtest.h>

// TODO: Fix this!
#include <TermDataInterface/SafeUuid.h>
#include <TermDataInterface/GroupUuid.h>
#include <TermDataInterface/TermUuid.h>

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
