// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

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
