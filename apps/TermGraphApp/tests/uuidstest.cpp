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

#include "source/helpers/uuid/uuidtools.h"
#include "source/model/uuid/groupuuid.h"
#include "source/model/uuid/safeuuid.h"
#include "source/model/uuid/termuuid.h"

TEST(UuidsTest, Validator)
{
    EXPECT_TRUE(UuidTools::isValidUuidString("{94810de3-51b8-469e-b316-00248ffa2a45}"));
    EXPECT_TRUE(UuidTools::isValidUuidString("{94810DE3-51B8-469E-B316-00248FFA2A45}"));

    EXPECT_FALSE(UuidTools::isValidUuidString("94810de3-51b8-469e-b316-00248ffa2a45"));
    EXPECT_FALSE(UuidTools::isValidUuidString("94810DE3-51B8-469E-B316-00248FFA2A45"));
    EXPECT_FALSE(UuidTools::isValidUuidString(" 999-999 "));
    EXPECT_FALSE(UuidTools::isValidUuidString("{}"));
    EXPECT_FALSE(UuidTools::isValidUuidString(""));
}

TEST(UuidsTest, NoBracesValidator)
{
    EXPECT_TRUE(UuidTools::isValidUuidStringWithoutBraces("94810de3-51b8-469e-b316-00248ffa2a45"));
    EXPECT_TRUE(UuidTools::isValidUuidStringWithoutBraces("94810DE3-51B8-469E-B316-00248FFA2A45"));

    EXPECT_FALSE(UuidTools::isValidUuidStringWithoutBraces("{94810de3-51b8-469e-b316-00248ffa2a45}"));
    EXPECT_FALSE(UuidTools::isValidUuidStringWithoutBraces("{94810DE3-51B8-469E-B316-00248FFA2A45}"));
    EXPECT_FALSE(UuidTools::isValidUuidStringWithoutBraces(" 999-999 "));
    EXPECT_FALSE(UuidTools::isValidUuidStringWithoutBraces("{}"));
    EXPECT_FALSE(UuidTools::isValidUuidStringWithoutBraces(""));
}

TEST(UuidsTest, SafeTest)
{
    auto nullUuid = QUuid();
    auto someUuid = QUuid::createUuid();

    auto safe1 = SafeUuid::create(nullUuid.toString());
    EXPECT_FALSE(safe1.has_value());

    auto safe2 = SafeUuid::create("");
    EXPECT_FALSE(safe2.has_value());

    auto safe3 = SafeUuid::create(someUuid.toString());
    EXPECT_TRUE(safe3.has_value());
    EXPECT_EQ(safe3->toString(), someUuid.toString());
    EXPECT_EQ(safe3->get(), someUuid);

    auto str   = QString("94810de3-51b8-469e-b316-00248ffa2a45");
    auto safe4 = SafeUuid::create(str);
    EXPECT_TRUE(safe4.has_value());
    EXPECT_EQ(safe4->toString(), "{" + str + "}");

    // Must be compilable
    TermUuid  termU  = TermUuid::create(str).value();
    GroupUuid groupU = GroupUuid::create(str).value();
}
