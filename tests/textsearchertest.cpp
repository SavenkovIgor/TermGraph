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
        QString    str("abc{");
        TextCursor startCursor(str, 0);

        auto resultCursor = TextSearcher::find(startCursor, Direction::Right, CharTools::isBracket);
        QVERIFY(resultCursor.has_value());

        QCOMPARE(resultCursor->pos(), 3);
        //        SearchCondition condition();
    }

    void simpleLeftSearch() {}

    void complexRightSearch() {}

    void complexLeftSearch() {}
};

QTEST_APPLESS_MAIN(SearcherTest)

#include "textsearchertest.moc"
