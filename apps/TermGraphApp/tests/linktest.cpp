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

#include <vector>

#include <gtest/gtest.h>

#include "source/helpers/link/link.h"
#include "source/helpers/link/linksstring.h"

class LinkTest : public ::testing::Test
{
public:
    using Idxs = std::vector<int>; // Short for Indexes

    const QString linkedTextWithUuid  = " abc {Ab c|94810de3-51b8-469e-b316-00248ffa2a45} abc ";
    const QString linkedText          = " abc {Ab c} abc ";
    const QString multipleLinks       = " {abc} {bc} {c}";
    const QString invalidLinksString1 = "}{";
    const QString invalidLinksString2 = "}}{{";
    const QString invalidLinksString3 = "";
};

TEST_F(LinkTest, SelectLink)
{
    struct Data
    {
        QString src;
        int     startFrom;
        bool    result;
        int     leftPos;
        int     rightPos;
        bool    isEmpty;
        int     size;
    };

    std::vector<Data> data;

    data.push_back(
        {.src = "", .startFrom = 0, .result = false, .leftPos = -1, .rightPos = -1, .isEmpty = true, .size = 0});
    data.push_back(
        {.src = " ", .startFrom = 0, .result = false, .leftPos = -1, .rightPos = -1, .isEmpty = true, .size = 0});
    data.push_back(
        {.src = "  ", .startFrom = 1, .result = false, .leftPos = -1, .rightPos = -1, .isEmpty = true, .size = 0});
    data.push_back(
        {.src = "} {", .startFrom = 0, .result = false, .leftPos = -1, .rightPos = -1, .isEmpty = true, .size = 0});
    data.push_back(
        {.src = "} {", .startFrom = 1, .result = false, .leftPos = -1, .rightPos = -1, .isEmpty = true, .size = 0});
    data.push_back(
        {.src = "} {", .startFrom = 2, .result = false, .leftPos = -1, .rightPos = -1, .isEmpty = true, .size = 0});
    data.push_back(
        {.src = "} {", .startFrom = 3, .result = false, .leftPos = -1, .rightPos = -1, .isEmpty = true, .size = 0});
    data.push_back(
        {.src = "{} {}", .startFrom = 2, .result = false, .leftPos = -1, .rightPos = -1, .isEmpty = true, .size = 0});
    data.push_back(
        {.src = " {a}", .startFrom = 1, .result = false, .leftPos = -1, .rightPos = -1, .isEmpty = true, .size = 3});
    data.push_back(
        {.src = " { abc2 }", .startFrom = 2, .result = true, .leftPos = 1, .rightPos = 9, .isEmpty = false, .size = 8});
    data.push_back(
        {.src = " {a} {b}", .startFrom = 2, .result = true, .leftPos = 1, .rightPos = 4, .isEmpty = false, .size = 3});
    data.push_back(
        {.src = "{}ab", .startFrom = 1, .result = true, .leftPos = 0, .rightPos = 2, .isEmpty = false, .size = 2});
    data.push_back({.src       = " {ab|94810de3-51b8-469e-b316-00248ffa2a45}",
                    .startFrom = 2,
                    .result    = true,
                    .leftPos   = 1,
                    .rightPos  = 42,
                    .isEmpty   = false,
                    .size      = 41});
    data.push_back({.src       = " { ab |94810de3-51b8-469e-b316-00248ffa2a45}",
                    .startFrom = 2,
                    .result    = true,
                    .leftPos   = 1,
                    .rightPos  = 44,
                    .isEmpty   = false,
                    .size      = 43});

    for (const auto& d : data) {
        auto borders = Link::select(d.src, d.startFrom);
        EXPECT_EQ(borders.has_value(), d.result);

        if (borders.has_value()) {
            EXPECT_EQ(borders->left().pos(), d.leftPos);
            EXPECT_EQ(borders->right().pos(), d.rightPos);
            EXPECT_EQ(borders->isEmpty(), d.isEmpty);
            EXPECT_EQ(borders->size(), d.size);
        }
    }
}

TEST_F(LinkTest, LinkWithUuid)
{
    auto link = Link(linkedTextWithUuid, 5, 48);
    auto res  = link.createLinkWithUuid(QUuid());

    EXPECT_EQ(res, "{Ab c|00000000-0000-0000-0000-000000000000}");
    EXPECT_EQ(link.fullLink(), QString("{Ab c|94810de3-51b8-469e-b316-00248ffa2a45}"));
    EXPECT_EQ(link.text(), QString("Ab c"));
    EXPECT_EQ(link.textLower(), "ab c");
    EXPECT_EQ(link.hasUuid(), true);
    EXPECT_EQ(link.uuid(), QUuid::fromString(QString("{94810de3-51b8-469e-b316-00248ffa2a45}")));
}

TEST_F(LinkTest, LinksSize)
{
    auto text = LinksString(multipleLinks);

    EXPECT_EQ(text.links().size(), 3);
    EXPECT_EQ(text.links()[1].fullLink(), QString("{bc}"));
    EXPECT_EQ(text.links()[1].text(), QString("bc"));

    EXPECT_EQ(LinksString(invalidLinksString1).links().size(), 0);
    EXPECT_EQ(LinksString(invalidLinksString2).links().size(), 0);
    EXPECT_EQ(LinksString(invalidLinksString3).links().size(), 0);
}

TEST_F(LinkTest, ReplaceLink)
{
    auto lText = LinksString(multipleLinks);

    auto res = lText.replaceLink(1, "{aaa}");

    EXPECT_EQ(res, " {abc} {aaa} {c}");
}

TEST_F(LinkTest, IsValidLinksString)
{
    EXPECT_TRUE(LinksString::isValidLinksString(QString("")));
    EXPECT_TRUE(LinksString::isValidLinksString(QString("{}")));
    EXPECT_TRUE(LinksString::isValidLinksString(QString("{}{}{}{}{}{}")));
    EXPECT_TRUE(LinksString::isValidLinksString(QString("{a}{b}{c}   {},-{}{}")));

    EXPECT_FALSE(LinksString::isValidLinksString(QString("{")));
    EXPECT_FALSE(LinksString::isValidLinksString(QString("}")));
    EXPECT_FALSE(LinksString::isValidLinksString(QString("}{")));
    EXPECT_FALSE(LinksString::isValidLinksString(QString("a}b{c")));
    EXPECT_FALSE(LinksString::isValidLinksString(QString("a}b}c")));
    EXPECT_FALSE(LinksString::isValidLinksString(QString("a{b{c")));
    EXPECT_FALSE(LinksString::isValidLinksString(QString("{{}{}}")));
    EXPECT_FALSE(LinksString::isValidLinksString(QString("{{}{}}}")));
    EXPECT_FALSE(LinksString::isValidLinksString(QString("{{{{{{{{{{}}}}}}}}}}")));
}

TEST_F(LinkTest, IsCursorOnLink)
{
    struct Data
    {
        QString src;
        Idxs    indices;
        bool    result;
    };

    std::vector<Data> data;

    data.push_back({.src = "{}", .indices = Idxs{1}, .result = true});
    data.push_back({.src = "{  }", .indices = Idxs{2}, .result = true});
    data.push_back({.src = "a{a}a", .indices = Idxs{2, 3}, .result = true});

    data.push_back({.src = "", .indices = Idxs{-1, 0, 1}, .result = false});
    data.push_back({.src = " ", .indices = Idxs{-1, 0, 1, 2}, .result = false});
    data.push_back({.src = "{", .indices = Idxs{0, 1}, .result = false});
    data.push_back({.src = "}", .indices = Idxs{0, 1}, .result = false});
    data.push_back({.src = "{}", .indices = Idxs{-1, 0, 2, 3}, .result = false});
    data.push_back({.src = "}{", .indices = Idxs{0, 1, 2}, .result = false});
    data.push_back({.src = "} a {", .indices = Idxs{1}, .result = false});

    for (const auto& d : data) {
        for (const auto& i : d.indices) {
            EXPECT_EQ(Link::isCursorOnLink(d.src, i), d.result);
        }
    }
}

TEST_F(LinkTest, LinkAt)
{
    struct Data
    {
        QString src;
        int     index;
        int     start;
        int     end;
    };

    std::vector<Data> data;

    data.push_back({.src = "{a}{b}{}", .index = 0, .start = 0, .end = 3});
    data.push_back({.src = "{a}{b}{}", .index = 1, .start = 3, .end = 6});
    data.push_back({.src = "{a}{b}{}", .index = 2, .start = 6, .end = 8});
    data.push_back({.src = " {a} {b} {} ", .index = 0, .start = 1, .end = 4});
    data.push_back({.src = " {a} {b} {} ", .index = 1, .start = 5, .end = 8});
    data.push_back({.src = " {a} {b} {} ", .index = 2, .start = 9, .end = 11});
    data.push_back({.src = "{}", .index = 0, .start = 0, .end = 2});
    data.push_back({.src = "{asdf}", .index = 0, .start = 0, .end = 6});
    data.push_back({.src = " {   } ", .index = 0, .start = 1, .end = 6});

    for (const auto& d : data) {
        auto lString = LinksString(d.src);
        auto range   = lString.links()[d.index];

        EXPECT_EQ(range.left().pos(), d.start);
        EXPECT_EQ(range.right().pos(), d.end);
    }
}

TEST_F(LinkTest, AddLink)
{
    struct Data
    {
        QString src;
        Idxs    indices;
        QString result;
    };

    std::vector<Data> data;

    data.push_back({.src = "", .indices = Idxs{-1, 1}, .result = ""});
    data.push_back({.src = "", .indices = Idxs{0}, .result = "{}"});
    data.push_back({.src = " ", .indices = Idxs{0}, .result = "{} "});
    data.push_back({.src = " ", .indices = Idxs{1}, .result = " {}"});
    data.push_back({.src = "a", .indices = Idxs{-1, 2}, .result = "a"});
    data.push_back({.src = "a", .indices = Idxs{0, 1}, .result = "{a}"});
    data.push_back({.src = " a ", .indices = Idxs{1, 2}, .result = " {a} "});
    data.push_back({.src = " a ", .indices = Idxs{0}, .result = "{} a "});
    data.push_back({.src = " a ", .indices = Idxs{3}, .result = " a {}"});
    data.push_back({.src = " {a} ", .indices = Idxs{2, 3}, .result = " {a} "});
    data.push_back({.src = " {a} ", .indices = Idxs{1}, .result = " {}{a} "});
    data.push_back({.src = " {a} ", .indices = Idxs{4}, .result = " {a}{} "});
    data.push_back({.src = " aa bb ", .indices = Idxs{1}, .result = " {aa} bb "});
    data.push_back({.src = " aa   bb ", .indices = Idxs{7}, .result = " aa   {bb} "});
    data.push_back({.src = " aaaaa, ", .indices = Idxs{1}, .result = " {aaaaa}, "});

    for (const auto& d : data) {
        for (const auto& i : d.indices) {
            EXPECT_EQ(LinksString::addLink(d.src, i), d.result);
        }
    }
}

TEST_F(LinkTest, ExtendRight)
{
    struct Data
    {
        QString str;
        Idxs    indices;
        QString result;
    };

    std::vector<Data> data;

    data.push_back({.str = "", .indices = Idxs{0}, .result = ""});
    data.push_back({.str = " ", .indices = Idxs{0, 1}, .result = " "});
    data.push_back({.str = "a", .indices = Idxs{0, 1}, .result = "a"});
    data.push_back({.str = " a ", .indices = Idxs{0, 1, 2, 3}, .result = " a "});
    data.push_back({.str = "{a}", .indices = Idxs{0, 1, 2, 3}, .result = "{a}"});
    data.push_back({.str = " {a}", .indices = Idxs{2, 3}, .result = " {a}"});
    data.push_back({.str = "{a}a", .indices = Idxs{1, 2}, .result = "{aa}"});
    data.push_back({.str = "{a}a", .indices = Idxs{0, 3, 4}, .result = "{a}a"});
    data.push_back({.str = " a a ", .indices = Idxs{0, 1, 2, 3, 4, 5}, .result = " a a "});
    data.push_back({.str = " {a} ", .indices = Idxs{2, 3}, .result = " {a} "});
    data.push_back({.str = " {a}a ", .indices = Idxs{2, 3}, .result = " {aa} "});
    data.push_back({.str = "{a}{a}", .indices = Idxs{1, 2}, .result = "{a}{a}"});
    data.push_back({.str = " {a} a ", .indices = Idxs{2, 3}, .result = " {a a} "});
    data.push_back({.str = "{a} {a}", .indices = Idxs{1, 2}, .result = "{a} {a}"});
    data.push_back({.str = " aaaaa, ", .indices = Idxs{1}, .result = " aaaaa, "});
    data.push_back({.str = " {a } a ", .indices = Idxs{2, 3, 4}, .result = " {a  a} "});
    data.push_back({.str = " {aa} bb ", .indices = Idxs{2}, .result = " {aa bb} "});
    data.push_back({.str = " {a} aaaaa, ", .indices = Idxs{2}, .result = " {a aaaaa}, "});

    for (const auto& d : data) {
        for (const auto& i : d.indices) {
            EXPECT_EQ(LinksString::expandLinkRight(d.str, i), d.result);
        }
    }
}

TEST_F(LinkTest, RemoveLink)
{
    struct Data
    {
        QString str;
        Idxs    indices;
        QString result;
    };

    std::vector<Data> data;

    data.push_back({.str = "{}", .indices = Idxs{0, 2}, .result = "{}"});
    data.push_back({.str = "{}", .indices = Idxs{1}, .result = ""});
    data.push_back({.str = " a ", .indices = Idxs{0, 1, 2, 3}, .result = " a "});
    data.push_back({.str = "a{}", .indices = Idxs{2}, .result = "a"});
    data.push_back({.str = "a{}", .indices = Idxs{1}, .result = "a{}"});
    data.push_back({.str = "{}a", .indices = Idxs{0}, .result = "{}a"});
    data.push_back({.str = "{}a", .indices = Idxs{1}, .result = "a"});
    data.push_back({.str = " {} ", .indices = Idxs{2}, .result = "  "});
    data.push_back({.str = " {a } ", .indices = Idxs{2, 3, 4}, .result = " a  "});
    data.push_back({.str = " {abcd} ", .indices = Idxs{2}, .result = " abcd "});
    data.push_back({.str = " {abcd abcd} ", .indices = Idxs{2}, .result = " abcd abcd "});

    for (const auto& d : data) {
        for (const int index : d.indices) {
            EXPECT_EQ(LinksString::removeLink(d.str, index), d.result);
        }
    }
}

TEST_F(LinkTest, LinkNoUuid)
{
    auto link = Link(linkedText, 5, 11);

    EXPECT_EQ(link.createLinkWithUuid(QUuid()), QString("{Ab c|00000000-0000-0000-0000-000000000000}"));
    EXPECT_EQ(link.fullLink(), QString("{Ab c}"));
    EXPECT_EQ(link.text(), QString("Ab c"));
    EXPECT_EQ(link.textLower(), "ab c");
    EXPECT_FALSE(link.hasUuid());
}
