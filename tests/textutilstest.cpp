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

//#include <QCoreApplication>
//#include <QDebug>
//#include <QtTest>

//#include "source/Helpers/text/textutils.h"

//class TextUtilsTest : public QObject
//{
//    Q_OBJECT

//public:
//    TextUtilsTest()           = default;
//    ~TextUtilsTest() override = default;

//private slots:

//    void isDefinition_data()
//    {
//        QTest::addColumn<QString>("text");
//        QTest::addColumn<bool>("result");

//        QTest::newRow("case0") << "a-b" << true;
//        QTest::newRow("case1") << "ab" << false;
//        QTest::newRow("case2") << "" << false;
//    }

//    void isDefinition()
//    {
//        QFETCH(QString, text);
//        QFETCH(bool, result);
//        QCOMPARE(TextUtils::isTermWithDefinition(text), result);
//    }

//    void extractTerm_data()
//    {
//        QTest::addColumn<QString>("text");
//        QTest::addColumn<QString>("result");

//        // clang-format off
//        QTest::newRow("case0") << "a-b"     << "a";
//        QTest::newRow("case1") << " a - b " << "a";
//        QTest::newRow("case2") << " - b "   << "";
//        QTest::newRow("case3") << " a -"    << "a";
//        QTest::newRow("case4") << "asdf"    << "";
//        QTest::newRow("case4") << "asdf-"   << "asdf";
//        // clang-format on
//    }

//    void extractTerm()
//    {
//        QFETCH(QString, text);
//        QFETCH(QString, result);
//        QCOMPARE(TextUtils::getTerm(text), result);
//    }

//    void extractDefinition_data()
//    {
//        QTest::addColumn<QString>("text");
//        QTest::addColumn<QString>("result");

//        // clang-format off
//        QTest::newRow("case0") << "a-b"     << "b";
//        QTest::newRow("case1") << " a - b " << "b";
//        QTest::newRow("case2") << " - b "   << "b";
//        QTest::newRow("case3") << " a - "   << "";
//        QTest::newRow("case4") << "asdf"    << "";
//        // clang-format on
//    }

//    void extractDefinition()
//    {
//        QFETCH(QString, text);
//        QFETCH(QString, result);
//        QCOMPARE(TextUtils::getDefinition(text), result);
//    }

//    void insertNewLineInMiddle_data()
//    {
//        QTest::addColumn<QString>("text");
//        QTest::addColumn<QString>("result");

//        // clang-format off
//        QTest::newRow("case0") << "a b"   << "a\nb";
//        QTest::newRow("case1") << "aaa b" << "aaa\nb";
//        QTest::newRow("case2") << "a bbb" << "a\nbbb";
//        QTest::newRow("case3") << "aabb"  << "aabb";
//        // clang-format on
//    }

//    void insertNewLineInMiddle()
//    {
//        QFETCH(QString, text);
//        QFETCH(QString, result);
//        QCOMPARE(TextUtils::insertNewLineNearMiddle(text), result);
//    }

//    void wordsCount_data()
//    {
//        QTest::addColumn<QString>("src");
//        QTest::addColumn<int>("count");

//        // Предполагается что -1 - влево, 1 - вправо
//        // В пустой строке поиск и в лево и вправо должен вовращать 0
//        // clang-format off
//        QTest::newRow("case0")  << ""                 << 0;
//        QTest::newRow("case1")  << " "                << 0;
//        QTest::newRow("case2")  << "     "            << 0;
//        QTest::newRow("case3")  << "w"                << 1;
//        QTest::newRow("case4")  << " w "              << 1;
//        QTest::newRow("case5")  << "2"                << 1;
//        QTest::newRow("case6")  << " 2 "              << 1;
//        QTest::newRow("case7")  << "w w w"            << 3;
//        QTest::newRow("case8")  << "abcd"             << 1;
//        QTest::newRow("case9")  << " abcd "           << 1;
//        QTest::newRow("case10") << "abcd abcd"        << 2;
//        QTest::newRow("case11") << " abcd abcd "      << 2;
//        QTest::newRow("case12") << "   abcd  abcd   " << 2;
//        // clang-format on
//    }

//    void wordsCount()
//    {
//        QFETCH(QString, src);
//        QFETCH(int, count);

//        QCOMPARE(TextUtils::wordCount(src), count);
//    }
//};

//QTEST_APPLESS_MAIN(TextUtilsTest)

//#include "textutilstest.moc"

#include <gtest/gtest.h>
#include <iostream>

using namespace std;

TEST(AR_TEST, TEST1) { ASSERT_TRUE(2 == 2); }

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
