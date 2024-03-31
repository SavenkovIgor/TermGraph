// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/Text/TextRange.h"

#include "source/Text/CheckingTextCursor.h"

TextRange::TextRange(QStringView view, int left, int right)
    : mString(view)
    , mLeftCursor(view, left)
    , mRightCursor(view, right)
{
    assert(left <= right);
    assert(left >= 0);
    assert(right >= 0);
}

const TextCursor &TextRange::left() const { return mLeftCursor; }

const TextCursor &TextRange::right() const { return mRightCursor; }

int TextRange::size() const { return mRightCursor.pos() - mLeftCursor.pos(); }

bool TextRange::isEmpty() const { return mLeftCursor.pos() == mRightCursor.pos(); }

std::pair<QString, int> TextRange::cutted() const
{
    auto ret = mString.toString();
    ret.remove(mLeftCursor.pos(), size());
    return {ret, mLeftCursor.pos()};
}

QStringView TextRange::rangeView() const { return mString.mid(left().pos(), size()); }

TextRange TextRange::selectWord(QStringView str, int startPos)
{
    auto lWord = CheckingTextCursor::leftWordBorder(str, startPos, Direction::Left);
    auto rWord = CheckingTextCursor::rightWordBorder(str, startPos, Direction::Right);

    if (lWord.check() && rWord.check()) {
        return {str, lWord.pos(), rWord.pos()};
    }

    return {str, startPos, startPos};
}
