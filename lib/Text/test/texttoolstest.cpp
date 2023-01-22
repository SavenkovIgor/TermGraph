// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include <Text/TextTools.h>

TEST(TextTools, WithDefinition)
{
    EXPECT_EQ(TextTools::isTermWithDefinition("a-b"), true);
    EXPECT_EQ(TextTools::isTermWithDefinition("ab"), false);
    EXPECT_EQ(TextTools::isTermWithDefinition(""), false);
}

TEST(TextTools, ExtractTerm)
{
    EXPECT_EQ(TextTools::getTerm("a-b"), "a");
    EXPECT_EQ(TextTools::getTerm("a-b"), "a");
    EXPECT_EQ(TextTools::getTerm(" a - b "), "a");
    EXPECT_EQ(TextTools::getTerm(" - b "), "");
    EXPECT_EQ(TextTools::getTerm(" a -"), "a");
    EXPECT_EQ(TextTools::getTerm("asdf"), "");
    EXPECT_EQ(TextTools::getTerm("asdf-"), "asdf");
}

TEST(TextTools, GetDefinition)
{
    EXPECT_EQ(TextTools::getDefinition("a-b"), "b");
    EXPECT_EQ(TextTools::getDefinition(" a - b "), "b");
    EXPECT_EQ(TextTools::getDefinition(" - b "), "b");
    EXPECT_EQ(TextTools::getDefinition(" a - "), "");
    EXPECT_EQ(TextTools::getDefinition("asdf"), "");
}

TEST(TextTools, WordCount)
{
    EXPECT_EQ(TextTools::wordCount(""), 0);
    EXPECT_EQ(TextTools::wordCount(" "), 0);
    EXPECT_EQ(TextTools::wordCount("     "), 0);
    EXPECT_EQ(TextTools::wordCount("w"), 1);
    EXPECT_EQ(TextTools::wordCount(" w "), 1);
    EXPECT_EQ(TextTools::wordCount("2"), 1);
    EXPECT_EQ(TextTools::wordCount(" 2 "), 1);
    EXPECT_EQ(TextTools::wordCount("w w w"), 3);
    EXPECT_EQ(TextTools::wordCount("abcd"), 1);
    EXPECT_EQ(TextTools::wordCount(" abcd "), 1);
    EXPECT_EQ(TextTools::wordCount("abcd abcd"), 2);
    EXPECT_EQ(TextTools::wordCount(" abcd abcd "), 2);
    EXPECT_EQ(TextTools::wordCount("   abcd  abcd   "), 2);
}
