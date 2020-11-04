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

#include "source/Helpers/text/checkingtextcursor.h"
#include "source/Helpers/text/textcursor.h"

// TODO: replace all opt<QChar> with just QChar.isNull check
// TODO: replace TextSearch with cursors

class CursorTest : public QObject
{
    Q_OBJECT

private slots:
    void initCheck()
    {
        QString str("abc");

        auto cursor = TextCursor(str, 1);
        QCOMPARE(cursor.pos(), 1);
        QCOMPARE(cursor.left(), 'a');
        QCOMPARE(cursor.right(), 'b');

        auto emptyCursor = TextCursor(QString());
        QCOMPARE(emptyCursor.pos(), 0);
        QCOMPARE(emptyCursor.left(), std::nullopt);
        QCOMPARE(emptyCursor.right(), std::nullopt);
    }

    void moveCursorLeft()
    {
        QString str("abc");

        auto cursor = TextCursor(str, 1);

        QCOMPARE(cursor.left(), 'a');
        QCOMPARE(cursor.right(), 'b');

        cursor--;

        QCOMPARE(cursor.left(), std::nullopt);
        QCOMPARE(cursor.right(), 'a');

        cursor--;

        QCOMPARE(cursor.left(), std::nullopt);
        QCOMPARE(cursor.right(), 'a');
    }

    void moveCursorRight()
    {
        QString str("abc");

        auto cursor = TextCursor(str, 2);

        QCOMPARE(cursor.left(), 'b');
        QCOMPARE(cursor.right(), 'c');

        cursor++;

        QCOMPARE(cursor.left(), 'c');
        QCOMPARE(cursor.right(), std::nullopt);

        cursor++;

        QCOMPARE(cursor.left(), 'c');
        QCOMPARE(cursor.right(), std::nullopt);
    }

    void altMove()
    {
        QString str("a");

        auto cursor = TextCursor(str);

        QCOMPARE(cursor.left(), std::nullopt);
        QCOMPARE(cursor.right(), 'a');

        cursor.move(Direction::Right);

        QCOMPARE(cursor.left(), 'a');
        QCOMPARE(cursor.right(), std::nullopt);

        cursor.move(Direction::Left);

        QCOMPARE(cursor.left(), std::nullopt);
        QCOMPARE(cursor.right(), 'a');
    }

    void emptyString()
    {
        QString str("");
        auto    cursor = TextCursor(str);

        QCOMPARE(cursor.left(), std::nullopt);
        QCOMPARE(cursor.right(), std::nullopt);
    }

    void bordersCheck()
    {
        QString empty("");
        auto    cur = TextCursor(empty);
        QCOMPARE(cur.atStart(), true);
        QCOMPARE(cur.atEnd(), true);
        QCOMPARE(cur.atBorder(), true);

        QString str("aa");
        auto    cur2 = TextCursor(str);

        QCOMPARE(cur2.atStart(), true);
        QCOMPARE(cur2.atEnd(), false);
        QCOMPARE(cur2.atBorder(), true);

        cur2++;
        QCOMPARE(cur2.atStart(), false);
        QCOMPARE(cur2.atEnd(), false);
        QCOMPARE(cur2.atBorder(), false);

        cur2++;
        QCOMPARE(cur2.atStart(), false);
        QCOMPARE(cur2.atEnd(), true);
        QCOMPARE(cur2.atBorder(), true);
    }

    void posCheck()
    {
        QString str("a");
        auto    cursor = TextCursor(str);
        QCOMPARE(cursor.pos(), 0);
        cursor.move(Direction::Right);
        QCOMPARE(cursor.pos(), 1);
        cursor++;
        QCOMPARE(cursor.pos(), 1);
        cursor.move(Direction::Left);
        QCOMPARE(cursor.pos(), 0);
        cursor--;
        QCOMPARE(cursor.pos(), 0);
    }

    void moveCheck()
    {
        QString str("a");
        auto    cursor = TextCursor(str);
        QVERIFY(!cursor.canMove(Direction::Left));
        QVERIFY(cursor.canMove(Direction::Right));
        cursor++;
        QVERIFY(cursor.canMove(Direction::Left));
        QVERIFY(!cursor.canMove(Direction::Right));
    }

    void CheckingCursor()
    {
        QString            str(" abc a");
        CheckingTextCursor cursor(str, 1, CharTools::isLetterOrNumber, CharTools::notLetterOrNumber);

        QCOMPARE(cursor.check(), false);
        cursor++;
        cursor++;
        cursor++;
        QCOMPARE(cursor.check(), true);
        cursor++;
        QCOMPARE(cursor.check(), false);
        cursor++;
        QCOMPARE(cursor.check(), true);
    }

    void stdCheckingCursors()
    {
        QString str(" abc a");
        auto    lbCursor = CheckingTextCursor::rightWordBorder(str, 1);
        QCOMPARE(lbCursor.check(), false);
        lbCursor++;
        lbCursor++;
        lbCursor++;
        QCOMPARE(lbCursor.check(), true);
        lbCursor++;
        QCOMPARE(lbCursor.check(), false);
        lbCursor++;
        QCOMPARE(lbCursor.check(), true);

        QString str2("ac a");
        auto    rbCursor = CheckingTextCursor::leftWordBorder(str2, 3);
        QCOMPARE(rbCursor.check(), true);
        rbCursor--;
        QCOMPARE(rbCursor.check(), false);
        rbCursor--;
        QCOMPARE(rbCursor.check(), false);
        rbCursor--;
        QCOMPARE(rbCursor.check(), true);
    }

    void steppingCursor()
    {
        QString str(" {abc} a");
        auto    lCursor = CheckingTextCursor::leftBracketOnRight(str);
        QCOMPARE(lCursor.search(Direction::Right), true);
        QCOMPARE(lCursor.pos(), 1);
        QCOMPARE(lCursor.check(), true);
        lCursor++;
        QCOMPARE(lCursor.check(), false);

        QCOMPARE(lCursor.search(Direction::Right), false);
        QCOMPARE(lCursor.pos(), 8);
        QCOMPARE(lCursor.check(), false);

        auto rCursor = CheckingTextCursor::rightBracketOnLeft(str, 8);
        QCOMPARE(rCursor.check(), false);
        QCOMPARE(rCursor.search(Direction::Left), true);
        QCOMPARE(rCursor.pos(), 6);
        QCOMPARE(rCursor.check(), true);
        rCursor--;
        QCOMPARE(rCursor.check(), false);

        QCOMPARE(rCursor.search(Direction::Left), false);
        QCOMPARE(rCursor.pos(), 0);
        QCOMPARE(rCursor.check(), false);

        auto cursor = CheckingTextCursor::anyBracketOnRight(str);

        QCOMPARE(cursor.check(), false);
        QCOMPARE(cursor.search(Direction::Right), true);
        QCOMPARE(cursor.pos(), 1);
        QCOMPARE(cursor.check(), true);
        cursor++;
        QCOMPARE(cursor.search(Direction::Right), true);
        QCOMPARE(cursor.pos(), 5);
        QCOMPARE(cursor.check(), true);
        cursor++;
        QCOMPARE(cursor.search(Direction::Right), false);
        QCOMPARE(cursor.pos(), 8);
        QCOMPARE(cursor.check(), false);
    }
};

QTEST_APPLESS_MAIN(CursorTest)

#include "cursortest.moc"
