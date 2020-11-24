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

#include <limits>
#include <vector>

#include <QCoreApplication>
#include <QDebug>
#include <QtTest>

#include "source/Helpers/link/linkutils.h"

class LinkUtilsTest : public QObject
{
    Q_OBJECT

    using Idxs = std::vector<int>; // Short for Indexes

public:
    LinkUtilsTest()           = default;
    ~LinkUtilsTest() override = default;

private slots:
    void linkExtraction_data()
    {
        QTest::addColumn<QString>("src");
        QTest::addColumn<QStringList>("tags");

        // clang-format off
        QTest::newRow("ok0")    << "a{bc}"     << (QStringList() << "bc");
        QTest::newRow("ok1")    << "a{a}{b}"   << (QStringList() << "a" << "b");
        QTest::newRow("ok2")    << "a{bc\\}"   << (QStringList() << "bc\\");
        QTest::newRow("ok3")    << "a{bc}{bc}" << (QStringList() << "bc");

        QTest::newRow("error0") << "{{"        << QStringList();
        QTest::newRow("error1") << "}}"        << QStringList();
        QTest::newRow("error2") << "a{b"       << QStringList();
        QTest::newRow("error3") << "a{a\\"     << QStringList();
        QTest::newRow("error4") << "a{bc}}"    << QStringList();
        QTest::newRow("error5") << "\\}\\}"    << QStringList();
        QTest::newRow("error6") << "a{bc\\}}"  << QStringList();
        // clang-format on
    }

    void linkExtraction() {}

    void linkLengthSuitTerm_data()
    {
        QTest::addColumn<QString>("word1");
        QTest::addColumn<QString>("word2");
        QTest::addColumn<bool>("result");

        // clang-format off
        QTest::newRow("case0") << ""                     << ""                        << true;
        QTest::newRow("case1") << "атом"                 << "атомов"                  << true;
        QTest::newRow("case2") << "электрон"             << "электроны"               << true;
        QTest::newRow("case3") << "отрицательный заряд"  << "отрицательный заряд"     << true;
        QTest::newRow("case4") << "элементарная частица" << "элементарными частицами" << true;
        QTest::newRow("case5") << "конформация"          << "конформационный"         << true;
        QTest::newRow("case7") << "w w"                  << "wwwww"                   << true;

        QTest::newRow("case8") << "w"   << "wwwwww"       << false;
        QTest::newRow("case9") << "w w" << "wwwww wwwwww" << false;
        // clang-format on
    }

    void linkLengthSuitTerm()
    {
        QFETCH(QString, word1);
        QFETCH(QString, word2);
        QFETCH(bool, result);

        QVERIFY(LinkUtils::tagLengthSuitTerm(word1, word2) == result);
        QVERIFY(LinkUtils::tagLengthSuitTerm(word2, word1) == result);
    }

    void getLevDistance_data()
    {
        QTest::addColumn<QString>("word1");
        QTest::addColumn<QString>("word2");
        QTest::addColumn<int>("limit");
        QTest::addColumn<int>("distance");

        // clang-format off
        QTest::newRow("case0") << ""                     << ""                        << 9 << 0;
        QTest::newRow("case1") << "атом"                 << "атомов"                  << 9 << 2;
        QTest::newRow("case2") << "электрон"             << "электроны"               << 9 << 1;
        QTest::newRow("case3") << "отрицательный заряд"  << "отрицательный заряд"     << 9 << 0;
        QTest::newRow("case4") << "элементарная частица" << "элементарными частицами" << 9 << 5;
//        QTest::newRow("case5") << "элементарная частица" << "элементарными частицами" << 3 << max_int;
        QTest::newRow("case6") << "конформация"          << "конформационный"         << 9 << 5;
        QTest::newRow("case7") << "w w"                  << "wwwww"                   << 9 << 3;
        // clang-format on
    }

    void getLevDistance()
    {
        QFETCH(QString, word1);
        QFETCH(QString, word2);
        QFETCH(int, limit);
        QFETCH(int, distance);

        qDebug() << LinkUtils::getLevDistance(word1, word2, limit);
        QVERIFY(LinkUtils::getLevDistance(word1, word2, limit) == distance);
    }

    void wordsCount_data()
    {
        QTest::addColumn<QString>("src");
        QTest::addColumn<int>("count");

        // Предполагается что -1 - влево, 1 - вправо
        // В пустой строке поиск и в лево и вправо должен вовращать 0
        // clang-format off
        QTest::newRow("case0")  << ""                 << 0;
        QTest::newRow("case1")  << "w"                << 1;
        QTest::newRow("case2")  << " w "              << 1;
        QTest::newRow("case3")  << "2"                << 1;
        QTest::newRow("case4")  << " 2 "              << 1;
        QTest::newRow("case5")  << "w w w"            << 3;
        QTest::newRow("case6")  << "abcd"             << 1;
        QTest::newRow("case7")  << " abcd "           << 1;
        QTest::newRow("case8")  << "abcd abcd"        << 2;
        QTest::newRow("case9")  << " abcd abcd "      << 2;
        QTest::newRow("case10") << "   abcd  abcd   " << 2;
        // clang-format on
    }

    void wordsCount()
    {
        QFETCH(QString, src);
        QFETCH(int, count);

        QVERIFY(LinkUtils::wordsCount(src) == count);
    }
};

QTEST_APPLESS_MAIN(LinkUtilsTest)

#include "linkutilstest.moc"
