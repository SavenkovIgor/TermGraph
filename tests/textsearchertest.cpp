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

#include <memory>

#include <QCoreApplication>
#include <QtTest>

#include "source/Helpers/text/chartools.h"
#include "source/Helpers/text/textcursor.h"
#include "source/Helpers/text/textsearcher.h"

class SearcherTest : public QObject
{
    Q_OBJECT

private slots:
    void simpleRigthSearch()
    {
        QString str("abc{");
        auto    cursor = TextCursor(str);

        auto resultCursor = TextSearcher::find(cursor, Direction::Right, CharTools::isLeftBracket);

        QVERIFY(resultCursor.has_value());
        QCOMPARE(resultCursor->pos(), 3);

        auto cursor2 = TextCursor(str, 3);

        auto resultCursor2 = TextSearcher::find(cursor2, Direction::Right, CharTools::isLeftBracket);

        QVERIFY(resultCursor2.has_value());
        QCOMPARE(resultCursor2->pos(), 3);
    }

    void simpleLeftSearch()
    {
        QString str("}abc{");
        auto    cursor = TextCursor(str, 2);

        auto resultCursor = TextSearcher::find(cursor, Direction::Left, CharTools::isRightBracket);

        QVERIFY(resultCursor.has_value());
        QCOMPARE(resultCursor->pos(), 1);
    }

    void failSearch()
    {
        QString str(" {abc ");
        auto    cursor = TextCursor(str, 2);

        auto resultCursor = TextSearcher::find(cursor, Direction::Right, CharTools::isLeftBracket);

        QVERIFY(!resultCursor.has_value());
    }

    void wordBorder_data()
    {
        QTest::addColumn<QString>("src");
        QTest::addColumn<int>("startFrom");
        QTest::addColumn<int>("leftPos");
        QTest::addColumn<int>("rightPos");
        QTest::addColumn<bool>("isEmpty");
        QTest::addColumn<int>("size");

        // clang-format off
        QTest::newRow("case0")  << ""      << 0 << 0 << 0 << true  << 0;
        QTest::newRow("case1")  << " "     << 0 << 0 << 0 << true  << 0;
        QTest::newRow("case2")  << " "     << 1 << 1 << 1 << true  << 0;
        QTest::newRow("case3")  << "  "    << 1 << 1 << 1 << true  << 0;
        QTest::newRow("case4")  << " a"    << 0 << 0 << 0 << true  << 0;
        QTest::newRow("case5")  << " a"    << 1 << 1 << 2 << false << 1;
        QTest::newRow("case6")  << "ab"    << 0 << 0 << 2 << false << 2;
        QTest::newRow("case7")  << "ab"    << 1 << 0 << 2 << false << 2;
        QTest::newRow("case8")  << "ab"    << 2 << 0 << 2 << false << 2;
        QTest::newRow("case9")  << " ab "  << 0 << 0 << 0 << true  << 0;
        QTest::newRow("case10") << " ab "  << 1 << 1 << 3 << false << 2;
        QTest::newRow("case11") << " ab "  << 2 << 1 << 3 << false << 2;
        QTest::newRow("case12") << " ab "  << 3 << 1 << 3 << false << 2;
        QTest::newRow("case13") << " ab "  << 4 << 4 << 4 << true  << 0;
        QTest::newRow("case14") << " ab, " << 1 << 1 << 3 << false << 2;
        QTest::newRow("case15") << " ab, " << 3 << 1 << 3 << false << 2;
        QTest::newRow("case16") << " ab, " << 4 << 4 << 4 << true  << 0;
        QTest::newRow("case17") << " a a " << 1 << 1 << 2 << false << 1;
        QTest::newRow("case18") << " a a " << 2 << 1 << 2 << false << 1;
        QTest::newRow("case19") << " a a " << 3 << 3 << 4 << false << 1;
        QTest::newRow("case20") << " a a " << 4 << 3 << 4 << false << 1;
        QTest::newRow("case21") << " a2a " << 4 << 1 << 4 << false << 3;
        // clang-format on
    }

    void wordBorder()
    {
        QFETCH(QString, src);
        QFETCH(int, startFrom);
        QFETCH(int, leftPos);
        QFETCH(int, rightPos);
        QFETCH(bool, isEmpty);
        QFETCH(int, size);

        auto borders = TextSearcher::selectWord(src, startFrom);

        QCOMPARE(borders.left().pos(), leftPos);
        QCOMPARE(borders.right().pos(), rightPos);
        QCOMPARE(borders.isEmpty(), isEmpty);
        QCOMPARE(borders.size(), size);
    }

    void linkSearch_data()
    {
        QTest::addColumn<QString>("src");
        QTest::addColumn<int>("startFrom");
        QTest::addColumn<bool>("result");

        QTest::addColumn<int>("leftPos");
        QTest::addColumn<int>("rightPos");
        QTest::addColumn<bool>("isEmpty");
        QTest::addColumn<int>("size");

        // clang-format off
        QTest::newRow("case01") << ""                                             << 0 << false << -1 << -1 << true  << 0;
        QTest::newRow("case02") << " "                                            << 0 << false << -1 << -1 << true  << 0;
        QTest::newRow("case03") << "  "                                           << 1 << false << -1 << -1 << true  << 0;
        QTest::newRow("case04") << "} {"                                          << 0 << false << -1 << -1 << true  << 0;
        QTest::newRow("case05") << "} {"                                          << 1 << false << -1 << -1 << true  << 0;
        QTest::newRow("case06") << "} {"                                          << 2 << false << -1 << -1 << true  << 0;
        QTest::newRow("case07") << "} {"                                          << 3 << false << -1 << -1 << true  << 0;
        QTest::newRow("case08") << "{} {}"                                        << 2 << false << -1 << -1 << true  << 0;
        QTest::newRow("case09") << " {a}"                                         << 1 << false << -1 << -1 << true  << 3;
        QTest::newRow("case10") << " { abc2 }"                                    << 2 << true  <<  1 <<  9 << false << 8;
        QTest::newRow("case11") << " {a} {b}"                                     << 2 << true  <<  1 <<  4 << false << 3;
        QTest::newRow("case12") << "{}ab"                                         << 1 << true  <<  0 <<  2 << false << 2;
        QTest::newRow("case13") << " {ab|94810de3-51b8-469e-b316-00248ffa2a45}"   << 2 << true  <<  1 << 42 << false << 41;
        QTest::newRow("case14") << " { ab |94810de3-51b8-469e-b316-00248ffa2a45}" << 2 << true  <<  1 << 44 << false << 43;
        // clang-format on
    }

    void linkSearch()
    {
        QFETCH(QString, src);
        QFETCH(int, startFrom);
        QFETCH(bool, result);
        QFETCH(int, leftPos);
        QFETCH(int, rightPos);
        QFETCH(bool, isEmpty);
        QFETCH(int, size);

        auto borders = TextSearcher::selectLink(src, startFrom);
        QCOMPARE(borders.has_value(), result);

        if (borders.has_value()) {
            QCOMPARE(borders->left().pos(), leftPos);
            QCOMPARE(borders->right().pos(), rightPos);
            QCOMPARE(borders->isEmpty(), isEmpty);
            QCOMPARE(borders->size(), size);
        }
    }
};

QTEST_APPLESS_MAIN(SearcherTest)

#include "textsearchertest.moc"
