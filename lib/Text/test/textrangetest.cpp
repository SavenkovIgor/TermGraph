// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <vector>

#include <gtest/gtest.h>

#include <Text/TextRange.h>

TEST(TextRangeTest, TextRangeInit)
{
    QString   str("ab");
    TextRange rng(str, 0, 1);

    EXPECT_EQ(rng.left().pos(), 0);
    EXPECT_EQ(rng.right().pos(), 1);
    EXPECT_FALSE(rng.isEmpty());
    EXPECT_EQ(rng.size(), 1);

    QString   str2;
    TextRange rng2(str2, 0, 0);

    EXPECT_EQ(rng2.left().left(), QChar());
    EXPECT_EQ(rng2.left().right(), QChar());
    EXPECT_EQ(rng2.right().left(), QChar());
    EXPECT_EQ(rng2.right().right(), QChar());
    EXPECT_TRUE(rng2.isEmpty());
    EXPECT_EQ(rng2.size(), 0);
}

TEST(TextRangeTest, RangeCut)
{
    QString   str(" abc b");
    TextRange rng(str, 1, 4);

    EXPECT_EQ(rng.cutted(), (std::pair<QString, int>("  b", 1)));
}

TEST(TextRangeTest, RangeView)
{
    QString   str(" abc ab ");
    TextRange rng(str, 3, 6);

    EXPECT_EQ(rng.rangeView(), QString("c a"));
}

TEST(TextRangeTest, WordBorder)
{
    struct Data
    {
        const char* word;
        const int   startFrom;
        const int   leftPos;
        const int   rightPos;
        const bool  isEmpty;
        const int   size;
    };

    std::vector<Data> inputs;

    inputs.push_back({.word = "", .startFrom = 0, .leftPos = 0, .rightPos = 0, .isEmpty = true, .size = 0});
    inputs.push_back({.word = " ", .startFrom = 0, .leftPos = 0, .rightPos = 0, .isEmpty = true, .size = 0});
    inputs.push_back({.word = " ", .startFrom = 1, .leftPos = 1, .rightPos = 1, .isEmpty = true, .size = 0});
    inputs.push_back({.word = "  ", .startFrom = 1, .leftPos = 1, .rightPos = 1, .isEmpty = true, .size = 0});
    inputs.push_back({.word = " a", .startFrom = 0, .leftPos = 0, .rightPos = 0, .isEmpty = true, .size = 0});
    inputs.push_back({.word = " a", .startFrom = 1, .leftPos = 1, .rightPos = 2, .isEmpty = false, .size = 1});
    inputs.push_back({.word = "ab", .startFrom = 0, .leftPos = 0, .rightPos = 2, .isEmpty = false, .size = 2});
    inputs.push_back({.word = "ab", .startFrom = 1, .leftPos = 0, .rightPos = 2, .isEmpty = false, .size = 2});
    inputs.push_back({.word = "ab", .startFrom = 2, .leftPos = 0, .rightPos = 2, .isEmpty = false, .size = 2});
    inputs.push_back({.word = " ab ", .startFrom = 0, .leftPos = 0, .rightPos = 0, .isEmpty = true, .size = 0});
    inputs.push_back({.word = " ab ", .startFrom = 1, .leftPos = 1, .rightPos = 3, .isEmpty = false, .size = 2});
    inputs.push_back({.word = " ab ", .startFrom = 2, .leftPos = 1, .rightPos = 3, .isEmpty = false, .size = 2});
    inputs.push_back({.word = " ab ", .startFrom = 3, .leftPos = 1, .rightPos = 3, .isEmpty = false, .size = 2});
    inputs.push_back({.word = " ab ", .startFrom = 4, .leftPos = 4, .rightPos = 4, .isEmpty = true, .size = 0});
    inputs.push_back({.word = " ab, ", .startFrom = 1, .leftPos = 1, .rightPos = 3, .isEmpty = false, .size = 2});
    inputs.push_back({.word = " ab, ", .startFrom = 3, .leftPos = 1, .rightPos = 3, .isEmpty = false, .size = 2});
    inputs.push_back({.word = " ab, ", .startFrom = 4, .leftPos = 4, .rightPos = 4, .isEmpty = true, .size = 0});
    inputs.push_back({.word = " a a ", .startFrom = 1, .leftPos = 1, .rightPos = 2, .isEmpty = false, .size = 1});
    inputs.push_back({.word = " a a ", .startFrom = 2, .leftPos = 1, .rightPos = 2, .isEmpty = false, .size = 1});
    inputs.push_back({.word = " a a ", .startFrom = 3, .leftPos = 3, .rightPos = 4, .isEmpty = false, .size = 1});
    inputs.push_back({.word = " a a ", .startFrom = 4, .leftPos = 3, .rightPos = 4, .isEmpty = false, .size = 1});
    inputs.push_back({.word = " a2a ", .startFrom = 4, .leftPos = 1, .rightPos = 4, .isEmpty = false, .size = 3});

    for (const auto& input : inputs) {
        auto borders = TextRange::selectWord(QString(input.word), input.startFrom);

        EXPECT_EQ(borders.left().pos(), input.leftPos);
        EXPECT_EQ(borders.right().pos(), input.rightPos);
        EXPECT_EQ(borders.isEmpty(), input.isEmpty);
        EXPECT_EQ(borders.size(), input.size);
    }
}
