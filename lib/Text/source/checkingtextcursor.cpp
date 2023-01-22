// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "include/Text/CheckingTextCursor.h"

CheckingTextCursor::CheckingTextCursor(QStringView                   strView,
                                       int                           pos,
                                       CheckingTextCursor::Condition leftChecker,
                                       CheckingTextCursor::Condition rightChecker,
                                       Opt<Direction>                autoStartSearch)
    : TextCursor(strView, pos)
    , mLeftChecker(leftChecker)
    , mRightChecker(rightChecker)
{
    if (autoStartSearch.has_value())
        search(autoStartSearch.value());
}

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

CheckingTextCursor CheckingTextCursor::rightWordBorder(QStringView strView, int pos, Opt<Direction> autoStartSearch)
{
    return CheckingTextCursor(strView, pos, CharTools::isLetterOrNumber, CharTools::notLetterOrNumber, autoStartSearch);
}

CheckingTextCursor CheckingTextCursor::leftWordBorder(QStringView strView, int pos, Opt<Direction> autoStartSearch)
{
    return CheckingTextCursor(strView, pos, CharTools::notLetterOrNumber, CharTools::isLetterOrNumber, autoStartSearch);
}

CheckingTextCursor CheckingTextCursor::leftBracketOnRight(QStringView strView, int pos, Opt<Direction> autoStartSearch)
{
    return CheckingTextCursor(strView, pos, CharTools::any, CharTools::isLeftBracket, autoStartSearch);
}

CheckingTextCursor CheckingTextCursor::rightBracketOnLeft(QStringView strView, int pos, Opt<Direction> autoStartSearch)
{
    return CheckingTextCursor(strView, pos, CharTools::isRightBracket, CharTools::any, autoStartSearch);
}

CheckingTextCursor CheckingTextCursor::anyBracketOnRight(QStringView strView, int pos, Opt<Direction> autoStartSearch)
{
    return CheckingTextCursor(strView, pos, CharTools::any, CharTools::isBracket, autoStartSearch);
}

CheckingTextCursor CheckingTextCursor::anyBracketOnLeft(QStringView strView, int pos, Opt<Direction> autoStartSearch)
{
    return CheckingTextCursor(strView, pos, CharTools::isBracket, CharTools::any, autoStartSearch);
}
