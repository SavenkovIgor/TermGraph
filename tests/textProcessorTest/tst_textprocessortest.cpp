/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
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

#include <QtTest>
#include <QCoreApplication>
#include <QDebug>

// add necessary includes here
#include "../../../TermGraph/source/Helpers/textprocessor.h"

class TextProcessorTest : public QObject
{
    Q_OBJECT

public:
    TextProcessorTest() = default;
    ~TextProcessorTest() override = default;

private slots:

    void isDefinition_data()
    {
        QTest::addColumn<QString>("text");
        QTest::addColumn<bool>("result");

        QTest::newRow("case0") << "a-b" << true;
        QTest::newRow("case1") << "ab" << false;
        QTest::newRow("case2") << "" << false;
    }

    void isDefinition()
    {
        QFETCH(QString, text);
        QFETCH(bool, result);
        QVERIFY(TextProcessor().isTermWithDefinition(text) == result);
    }

    void extractTerm_data()
    {
        QTest::addColumn<QString>("text");
        QTest::addColumn<QString>("result");

        QTest::newRow("case0") << "a-b" << "a";
        QTest::newRow("case1") << " a - b " << "a";
        QTest::newRow("case2") << " - b " << "";
        QTest::newRow("case3") << " a -" << "a";
        QTest::newRow("case4") << "asdf" << "";
        QTest::newRow("case4") << "asdf-" << "asdf";
    }

    void extractTerm()
    {
        QFETCH(QString, text);
        QFETCH(QString, result);
        QVERIFY(TextProcessor().getTerm(text) == result);
    }

    void extractDefinition_data()
    {
        QTest::addColumn<QString>("text");
        QTest::addColumn<QString>("result");

        QTest::newRow("case0") << "a-b" << "b";
        QTest::newRow("case1") << " a - b " << "b";
        QTest::newRow("case2") << " - b " << "b";
        QTest::newRow("case3") << " a - " << "";
        QTest::newRow("case4") << "asdf" << "";
    }

    void extractDefinition()
    {
        QFETCH(QString, text);
        QFETCH(QString, result);
        QVERIFY(TextProcessor().getDefinition(text) == result);
    }

    void insertNewLineInMiddle_data()
    {
        QTest::addColumn<QString>("text");
        QTest::addColumn<QString>("result");

        QTest::newRow("case0") << "a b" << "a\nb";
        QTest::newRow("case1") << "aaa b" << "aaa\nb";
        QTest::newRow("case2") << "a bbb" << "a\nbbb";
        QTest::newRow("case3") << "aabb" << "aabb";
    }

    void insertNewLineInMiddle()
    {
        QFETCH(QString, text);
        QFETCH(QString, result);
        QVERIFY(TextProcessor().insertNewLineNearMiddle(text) == result);
    }
};

QTEST_APPLESS_MAIN(TextProcessorTest)

#include "tst_textprocessortest.moc"
