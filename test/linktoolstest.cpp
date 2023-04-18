// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <limits>
#include <vector>

#include <gtest/gtest.h>

#include "source/helpers/link/linktools.h"

TEST(LinkToolsTest, LinkExtraction)
{
    struct Data
    {
        QString     src;
        QStringList links;
    };

    std::vector<Data> data;

    data.push_back({.src = "a{bc}", .links = (QStringList() << "bc")});
    data.push_back({.src   = "a{a}{b}",
                    .links = (QStringList() << "a"
                                            << "b")});
    data.push_back({.src = "a{bc\\}", .links = (QStringList() << "bc\\")});
    data.push_back({.src = "a{bc}{bc}", .links = (QStringList() << "bc")});
    data.push_back({.src = "{{", .links = QStringList()});
    data.push_back({.src = "}}", .links = QStringList()});
    data.push_back({.src = "a{b", .links = QStringList()});
    data.push_back({.src = "a{a\\", .links = QStringList()});
    data.push_back({.src = "a{bc}}", .links = QStringList()});
    data.push_back({.src = "\\}\\}", .links = QStringList()});
    data.push_back({.src = "a{bc\\}}", .links = QStringList()});

    // TODO: Finish test
}

TEST(LinkToolsTest, LinkLengthSuitTerm)
{
    struct Data
    {
        QString word1;
        QString word2;
        bool    result;
    };

    std::vector<Data> data;

    data.push_back({.word1 = "", .word2 = "", .result = true});
    data.push_back({.word1 = "атом", .word2 = "атомов", .result = true});
    data.push_back({.word1 = "электрон", .word2 = "электроны", .result = true});
    data.push_back({.word1 = "отрицательный заряд", .word2 = "отрицательный заряд", .result = true});
    data.push_back({.word1 = "элементарная частица", .word2 = "элементарными частицами", .result = true});
    data.push_back({.word1 = "конформация", .word2 = "конформационный", .result = true});
    data.push_back({.word1 = "w w", .word2 = "wwwww", .result = true});
    data.push_back({.word1 = "w", .word2 = "wwwwww", .result = false});
    data.push_back({.word1 = "w w", .word2 = "wwwww wwwwww", .result = false});

    for (const auto& d : data) {
        EXPECT_EQ(LinkTools::linkAndTermSimilarWordDistance(d.word1, d.word2), d.result);
        EXPECT_EQ(LinkTools::linkAndTermSimilarWordDistance(d.word2, d.word1), d.result);
    }
}

void getLevDistance_data()
{
    EXPECT_EQ(LinkTools::levDistance(QString(""), QString(""), 9), 0);
    EXPECT_EQ(LinkTools::levDistance(QString("атом"), QString("атомов"), 9), 2);
    EXPECT_EQ(LinkTools::levDistance(QString("электрон"), QString("электроны"), 9), 1);
    EXPECT_EQ(LinkTools::levDistance(QString("отрицательный заряд"), QString("отрицательный заряд"), 9), 0);
    EXPECT_EQ(LinkTools::levDistance(QString("элементарная частица"), QString("элементарными частицами"), 9), 5);
    EXPECT_EQ(LinkTools::levDistance(QString("конформация"), QString("конформационный"), 9), 5);
    EXPECT_EQ(LinkTools::levDistance(QString("w w"), QString("wwwww"), 9), 3);
}
