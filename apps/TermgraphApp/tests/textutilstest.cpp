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

#include "source/helpers/text/textutils.h"

TEST(TextUtils, WithDefinition)
{
    EXPECT_EQ(TextUtils::isTermWithDefinition("a-b"), true);
    EXPECT_EQ(TextUtils::isTermWithDefinition("ab"), false);
    EXPECT_EQ(TextUtils::isTermWithDefinition(""), false);
}

TEST(TextUtils, ExtractTerm)
{
    EXPECT_EQ(TextUtils::getTerm("a-b"), "a");
    EXPECT_EQ(TextUtils::getTerm("a-b"), "a");
    EXPECT_EQ(TextUtils::getTerm(" a - b "), "a");
    EXPECT_EQ(TextUtils::getTerm(" - b "), "");
    EXPECT_EQ(TextUtils::getTerm(" a -"), "a");
    EXPECT_EQ(TextUtils::getTerm("asdf"), "");
    EXPECT_EQ(TextUtils::getTerm("asdf-"), "asdf");
}

TEST(TextUtils, GetDefinition)
{
    EXPECT_EQ(TextUtils::getDefinition("a-b"), "b");
    EXPECT_EQ(TextUtils::getDefinition(" a - b "), "b");
    EXPECT_EQ(TextUtils::getDefinition(" - b "), "b");
    EXPECT_EQ(TextUtils::getDefinition(" a - "), "");
    EXPECT_EQ(TextUtils::getDefinition("asdf"), "");
}

TEST(TextUtils, InserNewLineNearMiddle)
{
    EXPECT_EQ(TextUtils::insertNewLineNearMiddle("a b"), "a\nb");
    EXPECT_EQ(TextUtils::insertNewLineNearMiddle("aaa b"), "aaa\nb");
    EXPECT_EQ(TextUtils::insertNewLineNearMiddle("a bbb"), "a\nbbb");
    EXPECT_EQ(TextUtils::insertNewLineNearMiddle("aabb"), "aabb");
}

TEST(TextUtils, WordCount)
{
    EXPECT_EQ(TextUtils::wordCount(""), 0);
    EXPECT_EQ(TextUtils::wordCount(" "), 0);
    EXPECT_EQ(TextUtils::wordCount("     "), 0);
    EXPECT_EQ(TextUtils::wordCount("w"), 1);
    EXPECT_EQ(TextUtils::wordCount(" w "), 1);
    EXPECT_EQ(TextUtils::wordCount("2"), 1);
    EXPECT_EQ(TextUtils::wordCount(" 2 "), 1);
    EXPECT_EQ(TextUtils::wordCount("w w w"), 3);
    EXPECT_EQ(TextUtils::wordCount("abcd"), 1);
    EXPECT_EQ(TextUtils::wordCount(" abcd "), 1);
    EXPECT_EQ(TextUtils::wordCount("abcd abcd"), 2);
    EXPECT_EQ(TextUtils::wordCount(" abcd abcd "), 2);
    EXPECT_EQ(TextUtils::wordCount("   abcd  abcd   "), 2);
}
