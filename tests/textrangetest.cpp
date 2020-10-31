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
#include "source/Helpers/text/textrange.h"
#include "source/Helpers/text/textsearcher.h"

// TODO: Move selectLink to TextSelector
// TODO: Move selectWord to TextSelector

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

        QCOMPARE(rng2.left().left(), std::nullopt);
        QCOMPARE(rng2.left().right(), std::nullopt);
        QCOMPARE(rng2.right().left(), std::nullopt);
        QCOMPARE(rng2.right().right(), std::nullopt);
        QCOMPARE(rng2.isEmpty(), true);
        QCOMPARE(rng2.size(), 0);
    }
};

QTEST_APPLESS_MAIN(TextRangeTest)

#include "textrangetest.moc"
