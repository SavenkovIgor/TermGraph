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

#include <gtest/gtest.h>

#include "source/Helpers/text/checkingtextcursor.h"
#include "source/Helpers/text/textcursor.h"

TEST (CursorTest, InitCheck) {
    QString str("abc");

    auto cursor = TextCursor(str, 1);
    EXPECT_EQ(cursor.pos(), 1);
    EXPECT_EQ(cursor.left(), 'a');
    EXPECT_EQ(cursor.right(), 'b');
    EXPECT_EQ(cursor.getSymbol(Direction::Left), 'a');
    EXPECT_EQ(cursor.getSymbol(Direction::Right), 'b');

    auto emptyCursor = TextCursor(QString());
    EXPECT_EQ(emptyCursor.pos(), 0);
    EXPECT_EQ(emptyCursor.left(), QChar());
    EXPECT_EQ(emptyCursor.right(), QChar());
}

TEST (CursorTest, MoveCursorLeft) {
    QString str("abc");

    auto cursor = TextCursor(str, 1);

    EXPECT_EQ(cursor.left(), 'a');
    EXPECT_EQ(cursor.right(), 'b');

    cursor--;

    EXPECT_EQ(cursor.left(), QChar());
    EXPECT_EQ(cursor.right(), 'a');

    cursor--;

    EXPECT_EQ(cursor.left(), QChar());
    EXPECT_EQ(cursor.right(), 'a');
}

TEST (CursorTest, MoveCursorRight) {
    QString str("abc");

    auto cursor = TextCursor(str, 2);

    EXPECT_EQ(cursor.left(), 'b');
    EXPECT_EQ(cursor.right(), 'c');

    cursor++;

    EXPECT_EQ(cursor.left(), 'c');
    EXPECT_EQ(cursor.right(), QChar());

    cursor++;

    EXPECT_EQ(cursor.left(), 'c');
    EXPECT_EQ(cursor.right(), QChar());
}

TEST (CursorTest, AltMove) {
    QString str("a");

    auto cursor = TextCursor(str);

    EXPECT_EQ(cursor.left(), QChar());
    EXPECT_EQ(cursor.right(), 'a');

    cursor.move(Direction::Right);

    EXPECT_EQ(cursor.left(), 'a');
    EXPECT_EQ(cursor.right(), QChar());

    cursor.move(Direction::Left);

    EXPECT_EQ(cursor.left(), QChar());
    EXPECT_EQ(cursor.right(), 'a');
}

TEST (CursorTest, EmptyString) {
    QString str("");
    auto    cursor = TextCursor(str);

    EXPECT_EQ(cursor.left(), QChar());
    EXPECT_EQ(cursor.right(), QChar());
}

TEST (CursorTest, BordersCheck) {
    QString empty("");
    auto    cur = TextCursor(empty);
    EXPECT_TRUE(cur.atStart());
    EXPECT_TRUE(cur.atEnd());
    EXPECT_TRUE(cur.atBorder());

    QString str("aa");
    auto    cur2 = TextCursor(str);

    EXPECT_TRUE(cur2.atStart());
    EXPECT_TRUE(cur2.atBorder());
    EXPECT_FALSE(cur2.atEnd());

    cur2++;
    EXPECT_FALSE(cur2.atStart());
    EXPECT_FALSE(cur2.atEnd());
    EXPECT_FALSE(cur2.atBorder());

    cur2++;
    EXPECT_FALSE(cur2.atStart());
    EXPECT_TRUE(cur2.atEnd());
    EXPECT_TRUE(cur2.atBorder());
}

TEST (CursorTest, PosCheck) {
    QString str("a");
    auto    cursor = TextCursor(str);
    EXPECT_EQ(cursor.pos(), 0);

    cursor.move(Direction::Right);
    EXPECT_EQ(cursor.pos(), 1);

    cursor++;
    EXPECT_EQ(cursor.pos(), 1);

    cursor.move(Direction::Left);
    EXPECT_EQ(cursor.pos(), 0);

    cursor--;
    EXPECT_EQ(cursor.pos(), 0);
}

TEST (CursorTest, MoveCheck) {
    QString str("a");
    auto    cursor = TextCursor(str);
    EXPECT_TRUE(!cursor.canMove(Direction::Left));
    EXPECT_TRUE(cursor.canMove(Direction::Right));

    cursor++;
    EXPECT_TRUE(cursor.canMove(Direction::Left));
    EXPECT_TRUE(!cursor.canMove(Direction::Right));
}

TEST (CursorTest, ValidCursor) {
    EXPECT_TRUE(TextCursor::isValidCursor(QString(""), 0));
    EXPECT_TRUE(TextCursor::isValidCursor(QString("a"), 1));
    EXPECT_TRUE(TextCursor::isValidCursor(QString("a"), 0));

    EXPECT_FALSE(TextCursor::isValidCursor(QString(""), -1));
    EXPECT_FALSE(TextCursor::isValidCursor(QString(""),  1));
    EXPECT_FALSE(TextCursor::isValidCursor(QString("a"), -1));
    EXPECT_FALSE(TextCursor::isValidCursor(QString("a"),  2));
}

TEST (CursorTest, CheckingCursor) {
    QString            str(" abc a");
    CheckingTextCursor cursor(str, 1, CharTools::isLetterOrNumber, CharTools::notLetterOrNumber);

    EXPECT_FALSE(cursor.check());
    cursor++;
    cursor++;
    cursor++;
    EXPECT_TRUE(cursor.check());
    cursor++;
    EXPECT_FALSE(cursor.check());
    cursor++;
    EXPECT_TRUE(cursor.check());
}

TEST (CursorTest, StdCheckingCursors) {
    QString str(" abc a");
    auto    lbCursor = CheckingTextCursor::rightWordBorder(str, 1);
    EXPECT_FALSE(lbCursor.check());
    lbCursor++;
    lbCursor++;
    lbCursor++;
    EXPECT_TRUE(lbCursor.check());
    lbCursor++;
    EXPECT_FALSE(lbCursor.check());
    lbCursor++;
    EXPECT_TRUE(lbCursor.check());

    QString str2("ac a");
    auto    rbCursor = CheckingTextCursor::leftWordBorder(str2, 3);
    EXPECT_TRUE(rbCursor.check());
    rbCursor--;
    EXPECT_FALSE(rbCursor.check());
    rbCursor--;
    EXPECT_FALSE(rbCursor.check());
    rbCursor--;
    EXPECT_TRUE(rbCursor.check());
}

TEST (CursorTest, SteppingCursor) {
    QString str(" {abc} a");
    auto    lCursor = CheckingTextCursor::leftBracketOnRight(str);
    EXPECT_TRUE(lCursor.search(Direction::Right));
    EXPECT_EQ(lCursor.pos(), 1);
    EXPECT_TRUE(lCursor.check());
    lCursor++;
    EXPECT_FALSE(lCursor.check());

    EXPECT_FALSE(lCursor.search(Direction::Right));
    EXPECT_EQ(lCursor.pos(), 8);
    EXPECT_FALSE(lCursor.check());

    auto rCursor = CheckingTextCursor::rightBracketOnLeft(str, 8);
    EXPECT_FALSE(rCursor.check());
    EXPECT_TRUE(rCursor.search(Direction::Left));
    EXPECT_EQ(rCursor.pos(), 6);
    EXPECT_TRUE(rCursor.check());
    rCursor--;
    EXPECT_FALSE(rCursor.check());

    EXPECT_FALSE(rCursor.search(Direction::Left));
    EXPECT_EQ(rCursor.pos(), 0);
    EXPECT_FALSE(rCursor.check());

    auto cursor = CheckingTextCursor::anyBracketOnRight(str);

    EXPECT_FALSE(cursor.check());
    EXPECT_TRUE(cursor.search(Direction::Right));
    EXPECT_EQ(cursor.pos(), 1);
    EXPECT_TRUE(cursor.check());
    cursor++;
    EXPECT_TRUE(cursor.search(Direction::Right));
    EXPECT_EQ(cursor.pos(), 5);
    EXPECT_TRUE(cursor.check());
    cursor++;
    EXPECT_FALSE(cursor.search(Direction::Right));
    EXPECT_EQ(cursor.pos(), 8);
    EXPECT_FALSE(cursor.check());
}

TEST (CursorTest, AutoSearchCursor) {
    QString str(" {abc} a");
    auto    lCursor = CheckingTextCursor::leftBracketOnRight(str, 0, Direction::Right);
    EXPECT_EQ(lCursor.pos(), 1);
    EXPECT_TRUE(lCursor.check());
    lCursor++;
    EXPECT_FALSE(lCursor.check());

    EXPECT_FALSE(lCursor.search(Direction::Right));
    EXPECT_EQ(lCursor.pos(), 8);
    EXPECT_FALSE(lCursor.check());

    auto rCursor = CheckingTextCursor::rightBracketOnLeft(str, 8, Direction::Left);
    EXPECT_EQ(rCursor.pos(), 6);
    EXPECT_TRUE(rCursor.check());
    rCursor--;
    EXPECT_FALSE(rCursor.check());

    EXPECT_FALSE(rCursor.search(Direction::Left));
    EXPECT_EQ(rCursor.pos(), 0);
    EXPECT_FALSE(rCursor.check());

    auto cursor = CheckingTextCursor::anyBracketOnRight(str, 0, Direction::Right);
    EXPECT_EQ(cursor.pos(), 1);
    EXPECT_TRUE(cursor.check());
    cursor++;
    EXPECT_TRUE(cursor.search(Direction::Right));
    EXPECT_EQ(cursor.pos(), 5);
    EXPECT_TRUE(cursor.check());
    cursor++;
    EXPECT_FALSE(cursor.search(Direction::Right));
    EXPECT_EQ(cursor.pos(), 8);
    EXPECT_FALSE(cursor.check());
}
