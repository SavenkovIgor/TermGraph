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

#include "source/Helpers/text/textcursor.h"

#include <algorithm>

TextCursor::TextCursor(QStringView strView, int pos)
    : mPos(std::clamp(pos, 0, static_cast<int>(strView.size())))
    , mString(strView)
{
    assert(mPos == pos);
}

bool TextCursor::move(Direction dir)
{
    assert(isValidCursor());

    if (canMove(dir)) {
        int offset = dir == Direction::Left ? -1 : 1;
        mPos += offset;
        return true;
    }
    return false;
}

bool TextCursor::move(Direction dir, CharTools::ShortCondition whileCond)
{
    do {
        auto character = getSymbol(dir);
        if (!character)
            return false;

        if (whileCond(character.value()))
            return true;

    } while (move(dir));

    return false;
}

bool TextCursor::move(Direction dir, CharTools::FullCondition fullCond)
{
    do {
        if (fullCond(left(), right()))
            return true;

    } while (move(dir));

    return false;
}

bool TextCursor::moveLeft(CharTools::ShortCondition whileCond) { return move(Direction::Left, whileCond); }

bool TextCursor::moveRight(CharTools::ShortCondition whileCond) { return move(Direction::Right, whileCond); }

bool TextCursor::move(Direction dir, const QChar &stopChar)
{
    auto cond = [stopChar](const QChar &ch) { return ch == stopChar; };
    return move(dir, cond);
}

bool TextCursor::canMove(Direction dir) const
{
    if (dir == Direction::Left)
        return mPos > 0;
    else
        return mPos < mString.size();
}

int TextCursor::pos() const { return mPos; }

opt<QChar> TextCursor::getSymbol(Direction dir) const
{
    assert(isValidCursor());

    if (!canMove(dir)) // Near border
        return std::nullopt;

    int offset = dir == Direction::Left ? -1 : 0;
    return mString[mPos + offset];
}

opt<QChar> TextCursor::left() const { return getSymbol(Direction::Left); }

opt<QChar> TextCursor::right() const { return getSymbol(Direction::Right); }

TextCursor &TextCursor::operator++(int)
{
    move(Direction::Right);
    return *this;
}

TextCursor &TextCursor::operator--(int)
{
    move(Direction::Left);
    return *this;
}
