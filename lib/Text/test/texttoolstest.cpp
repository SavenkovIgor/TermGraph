// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include <Text/TextTools.h>

TEST(TextTools, IsTermWithDefinition)
{
    EXPECT_EQ(TextTools::isTermWithDefinition("a - b"), true);
    EXPECT_EQ(TextTools::isTermWithDefinition("a - b - c"), true);
    EXPECT_EQ(TextTools::isTermWithDefinition("a-b - c"), true);
    EXPECT_EQ(TextTools::isTermWithDefinition("a - b-c"), true);

    EXPECT_EQ(TextTools::isTermWithDefinition("a-b -c"), false);
    EXPECT_EQ(TextTools::isTermWithDefinition("a-b-c"), false);
    EXPECT_EQ(TextTools::isTermWithDefinition("a-b"), false);
    EXPECT_EQ(TextTools::isTermWithDefinition("a-b"), false);
    EXPECT_EQ(TextTools::isTermWithDefinition("ab"), false);
    EXPECT_EQ(TextTools::isTermWithDefinition(""), false);
}

TEST(TextTools, ExtractTermAndDefinition)
{
    using ans = QPair<QString, QString>;
    EXPECT_EQ(TextTools::splitTermAndDefinition("a - b"), ans("a", "b"));
    EXPECT_EQ(TextTools::splitTermAndDefinition("a - b - c"), ans("a", "b - c"));
    EXPECT_EQ(TextTools::splitTermAndDefinition("a-b - c"), ans("a-b", "c"));
    EXPECT_EQ(TextTools::splitTermAndDefinition("a - b-c"), ans("a", "b-c"));
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
