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

#include "source/Helpers/text/checkingtextcursor.h"

CheckingTextCursor::CheckingTextCursor(QStringView                   strView,
                                       int                           pos,
                                       CheckingTextCursor::Condition leftChecker,
                                       CheckingTextCursor::Condition rightChecker)
    : TextCursor(strView, pos)
    , mLeftChecker(leftChecker)
    , mRightChecker(rightChecker)
{}

bool CheckingTextCursor::check() const { return mLeftChecker(left()) && mRightChecker(right()); }

bool CheckingTextCursor::search(Direction dir)
{
    do {
        if (check())
            return true;

        move(dir);
    } while (canMove(dir));

    return check();
}

CheckingTextCursor CheckingTextCursor::rightWordBorder(QStringView strView, int pos)
{
    return CheckingTextCursor(strView, pos, CharTools::isLetterOrNumber, CharTools::notLetterOrNumber);
}

CheckingTextCursor CheckingTextCursor::leftWordBorder(QStringView strView, int pos)
{
    return CheckingTextCursor(strView, pos, CharTools::notLetterOrNumber, CharTools::isLetterOrNumber);
}

CheckingTextCursor CheckingTextCursor::leftBracketOnRight(QStringView strView, int pos)
{
    return CheckingTextCursor(strView, pos, CharTools::any, CharTools::isLeftBracket);
}

CheckingTextCursor CheckingTextCursor::rightBracketOnLeft(QStringView strView, int pos)
{
    return CheckingTextCursor(strView, pos, CharTools::isRightBracket, CharTools::any);
}

CheckingTextCursor CheckingTextCursor::anyBracketOnRight(QStringView strView, int pos)
{
    return CheckingTextCursor(strView, pos, CharTools::any, CharTools::isBracket);
}

CheckingTextCursor CheckingTextCursor::anyBracketOnLeft(QStringView strView, int pos)
{
    return CheckingTextCursor(strView, pos, CharTools::isBracket, CharTools::any);
}
