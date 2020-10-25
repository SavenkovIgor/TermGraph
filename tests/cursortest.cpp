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

class CursorTest : public QObject
{
    Q_OBJECT

private slots:
    void initCheck()
    {
        QString str("abc");

        auto validCursor    = TextCursor::create(str, 0);
        auto invalidCursor  = TextCursor::create(str, -1);
        auto invalidCursor2 = TextCursor::create(str, -100);

        QVERIFY(validCursor.has_value());
        QVERIFY(validCursor->pos() == 0);

        QVERIFY(!invalidCursor.has_value());
        QVERIFY(!invalidCursor2.has_value());
    }

    void moveCursorLeft()
    {
        QString str("abc");

        auto cursor = TextCursor::create(str, 1).value();

        QVERIFY(cursor.left() == 'a');
        QVERIFY(cursor.right() == 'b');

        cursor--;

        QVERIFY(cursor.left() == std::nullopt);
        QVERIFY(cursor.right() == 'a');

        cursor--;

        QVERIFY(cursor.left() == std::nullopt);
        QVERIFY(cursor.right() == 'a');
    }

    void moveCursorRight()
    {
        QString str("abc");

        auto cursor = TextCursor::create(str, 2).value();

        QVERIFY(cursor.left() == 'b');
        QVERIFY(cursor.right() == 'c');

        cursor++;

        QVERIFY(cursor.left() == 'c');
        QVERIFY(cursor.right() == std::nullopt);

        cursor++;

        QVERIFY(cursor.left() == 'c');
        QVERIFY(cursor.right() == std::nullopt);
    }

    void altMove()
    {
        QString str("a");

        auto cursor = TextCursor::create(str, 0).value();

        QVERIFY(cursor.left() == std::nullopt);
        QVERIFY(cursor.right() == 'a');

        cursor.move(Direction::Right);

        QVERIFY(cursor.left() == 'a');
        QVERIFY(cursor.right() == std::nullopt);

        cursor.move(Direction::Left);

        QVERIFY(cursor.left() == std::nullopt);
        QVERIFY(cursor.right() == 'a');
    }

    void emptyString()
    {
        QString str("");
        auto    cursorOpt = TextCursor::create(str, 0);

        QVERIFY(cursorOpt.has_value());

        auto cursor = cursorOpt.value();

        QVERIFY(cursor.left() == std::nullopt);
        QVERIFY(cursor.right() == std::nullopt);
    }

    void posCheck()
    {
        QString str("a");
        auto    cursor = TextCursor::create(str, 0).value();
        QVERIFY(cursor.pos() == 0);
        cursor++;
        QVERIFY(cursor.pos() == 1);
        cursor++;
        QVERIFY(cursor.pos() == 1);
        cursor--;
        QVERIFY(cursor.pos() == 0);
        cursor--;
        QVERIFY(cursor.pos() == 0);
    }

    void moveCheck()
    {
        QString str("a");
        auto    cursor = TextCursor::create(str, 0).value();
        QVERIFY(!cursor.canMove(Direction::Left));
        QVERIFY(cursor.canMove(Direction::Right));
        cursor++;
        QVERIFY(cursor.canMove(Direction::Left));
        QVERIFY(!cursor.canMove(Direction::Right));
    }
};

QTEST_APPLESS_MAIN(CursorTest)

#include "cursortest.moc"
