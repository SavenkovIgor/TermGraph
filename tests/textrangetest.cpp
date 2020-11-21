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

#include "source/Helpers/text/textrange.h"

class TextRangeTest : public QObject
{
    Q_OBJECT

private slots:
    void textRangeInit()
    {
        QString   str("ab");
        TextRange rng(str, 0, 1);

        QCOMPARE(rng.left().pos(), 0);
        QCOMPARE(rng.right().pos(), 1);
        QCOMPARE(rng.isEmpty(), false);
        QCOMPARE(rng.size(), 1);

        QString   str2;
        TextRange rng2(str2, 0, 0);

        QCOMPARE(rng2.left().left(), QChar());
        QCOMPARE(rng2.left().right(), QChar());
        QCOMPARE(rng2.right().left(), QChar());
        QCOMPARE(rng2.right().right(), QChar());
        QCOMPARE(rng2.isEmpty(), true);
        QCOMPARE(rng2.size(), 0);
    }

    void rangeCut()
    {
        QString   str(" abc b");
        TextRange rng(str, 1, 4);

        QCOMPARE(rng.cutted(), (std::pair<QString, int>("  b", 1)));
    }

    void rangeView()
    {
        QString   str(" abc ab ");
        TextRange rng(str, 3, 6);

        QCOMPARE(rng.rangeView(), QString("c a"));
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

        auto borders = TextRange::selectWord(src, startFrom);

        QCOMPARE(borders.left().pos(), leftPos);
        QCOMPARE(borders.right().pos(), rightPos);
        QCOMPARE(borders.isEmpty(), isEmpty);
        QCOMPARE(borders.size(), size);
    }
};

QTEST_APPLESS_MAIN(TextRangeTest)

#include "textrangetest.moc"
