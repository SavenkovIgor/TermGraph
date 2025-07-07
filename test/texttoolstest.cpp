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
    EXPECT_EQ(TextTools::joinTermDef({u"a"}, {u"b"}), u"a - b");
    EXPECT_EQ(TextTools::joinTermDef({u"a"}, {u""}), u"a - ");
    EXPECT_EQ(TextTools::joinTermDef({u""}, {u"b"}), u" - b");
    EXPECT_EQ(TextTools::joinTermDef({u""}, {u""}), u" - ");
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
