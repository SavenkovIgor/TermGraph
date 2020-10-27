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

#include "source/Helpers/text/textcursor.h"

// TODO: Remove create
// TODO: Cursor must be always valid. clamp it to valid borders

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
};

QTEST_APPLESS_MAIN(CursorTest)

#include "cursortest.moc"
