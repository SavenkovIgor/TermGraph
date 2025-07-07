// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include "source/Text/TextTools.h"

TEST(TextTools, isTermAndDefinition)
{
    EXPECT_EQ(TextTools::isTermAndDefinition({u"a - b"}), true);
    EXPECT_EQ(TextTools::isTermAndDefinition({u"a - b - c"}), true);
    EXPECT_EQ(TextTools::isTermAndDefinition({u"a-b - c"}), true);
    EXPECT_EQ(TextTools::isTermAndDefinition({u"a - b-c"}), true);

    EXPECT_EQ(TextTools::isTermAndDefinition({u"a-b -c"}), false);
    EXPECT_EQ(TextTools::isTermAndDefinition({u"a-b-c"}), false);
    EXPECT_EQ(TextTools::isTermAndDefinition({u"a-b"}), false);
    EXPECT_EQ(TextTools::isTermAndDefinition({u"a-b"}), false);
    EXPECT_EQ(TextTools::isTermAndDefinition({u"ab"}), false);
    EXPECT_EQ(TextTools::isTermAndDefinition({u""}), false);
}

TEST(TextTools, ExtractTermAndDefinition)
{
    using ans = QPair<QString, QString>;
    EXPECT_EQ(TextTools::splitTermAndDefinition({u"a - b"}), ans(u"a", u"b"));
    EXPECT_EQ(TextTools::splitTermAndDefinition({u"a - b - c"}), ans(u"a", u"b - c"));
    EXPECT_EQ(TextTools::splitTermAndDefinition({u"a-b - c"}), ans(u"a-b", u"c"));
    EXPECT_EQ(TextTools::splitTermAndDefinition({u"a - b-c"}), ans(u"a", u"b-c"));
    EXPECT_EQ(TextTools::splitTermAndDefinition({u" - "}), ans(u"", u""));
    EXPECT_EQ(TextTools::splitTermAndDefinition({u"a - "}), ans(u"a", u""));
    EXPECT_EQ(TextTools::splitTermAndDefinition({u" - b"}), ans(u"", u"b"));
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
