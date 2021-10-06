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

#include "source/helpers/text/textrange.h"

#include "source/helpers/text/checkingtextcursor.h"

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
    auto cursor = TextCursor(str, startPos);

    auto lWord = CheckingTextCursor::leftWordBorder(str, startPos, Direction::Left);
    auto rWord = CheckingTextCursor::rightWordBorder(str, startPos, Direction::Right);

    if (lWord.check() && rWord.check())
        return TextRange(str, lWord.pos(), rWord.pos());

    return TextRange(str, startPos, startPos);
}