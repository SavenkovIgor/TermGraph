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

#include "source/helpers/text/textcursor.h"

#include <algorithm>

TextCursor::TextCursor(QStringView strView, int pos)
    : mPos(std::clamp(pos, 0, static_cast<int>(strView.size())))
    , mString(strView)
{
    assert(mPos == pos);
}

bool TextCursor::isValidCursor(QStringView view, int cursor) { return 0 <= cursor && cursor <= view.size(); }

int TextCursor::pos() const { return mPos; }

bool TextCursor::atStart() const { return mPos == 0; }

bool TextCursor::atEnd() const { return mPos == mString.size(); }

bool TextCursor::atBorder() const { return atStart() || atEnd(); }

QChar TextCursor::left() const { return getSymbol(Direction::Left); }

QChar TextCursor::right() const { return getSymbol(Direction::Right); }

QChar TextCursor::getSymbol(Direction dir) const
{
    if (!canMove(dir)) // Near border
        return QChar();

    int offset = dir == Direction::Left ? -1 : 0;
    return mString[mPos + offset];
}

bool TextCursor::canMove(Direction dir) const { return dir == Direction::Left ? !atStart() : !atEnd(); }

bool TextCursor::move(Direction dir)
{
    if (canMove(dir)) {
        int offset = dir == Direction::Left ? -1 : 1;
        mPos += offset;
        return true;
    }
    return false;
}

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
