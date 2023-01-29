// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "include/Text/TextCursor.h"

#include <algorithm>

#include <CommonTools/HandyTypes.h>

TextCursor::TextCursor(QStringView strView, int pos)
    : mPos(std::clamp(pos, 0, asInt(strView.size())))
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
