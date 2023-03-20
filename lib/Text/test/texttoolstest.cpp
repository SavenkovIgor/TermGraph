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
    EXPECT_EQ(TextTools::splitTermAndDefinition(" - "), ans("", ""));
    EXPECT_EQ(TextTools::splitTermAndDefinition("a - "), ans("a", ""));
    EXPECT_EQ(TextTools::splitTermAndDefinition(" - b"), ans("", "b"));
}

TEST(TextTools, JoinTermDef)
{
    EXPECT_EQ(TextTools::joinTermDef("a", "b"), "a - b");
    EXPECT_EQ(TextTools::joinTermDef("a", ""), "a - ");
    EXPECT_EQ(TextTools::joinTermDef("", "b"), " - b");
    EXPECT_EQ(TextTools::joinTermDef("", ""), " - ");
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
