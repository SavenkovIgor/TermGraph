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

    void complexRightSearch()
    {
        QString str("abc ");
        auto    cursor = TextCursor(str);

        auto resultCursor = TextSearcher::find(cursor, Direction::Right, CharTools::isLetterLeftAndNotLetterRight);

        QVERIFY(resultCursor.has_value());
        QCOMPARE(resultCursor->pos(), 3);

        QString str2("abc");
        auto    cur2 = TextCursor(str2);

        auto res2 = TextSearcher::find(cur2, Direction::Right, CharTools::isLetterLeftAndNotLetterRight);

        QVERIFY(res2.has_value());
        QCOMPARE(res2->pos(), 3);
    }

    void complexLeftSearch()
    {
        QString str(" {abc ");
        auto    cursor = TextCursor(str, 2);

        auto resultCursor = TextSearcher::find(cursor, Direction::Left, CharTools::isLeftBracketOnRight);

        QVERIFY(resultCursor.has_value());
        QCOMPARE(resultCursor->pos(), 1);

        QString str2("{abc");
        auto    cur2 = TextCursor(str2, 2);

        auto res2 = TextSearcher::find(cur2, Direction::Left, CharTools::isLeftBracketOnRight);

        QVERIFY(res2.has_value());
        QCOMPARE(res2->pos(), 0);
    }

    void failSearch()
    {
        QString str(" {abc ");
        auto    cursor = TextCursor(str, 2);

        auto resultCursor = TextSearcher::find(cursor, Direction::Right, CharTools::isLeftBracket);

        QVERIFY(!resultCursor.has_value());

        QString str2("");
        auto    cur2 = TextCursor(str2);

        auto res2 = TextSearcher::find(cur2, Direction::Left, CharTools::isLeftBracketOnRight);

        QVERIFY(!res2.has_value());
    }
};

QTEST_APPLESS_MAIN(SearcherTest)

#include "textsearchertest.moc"
