// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <vector>

#include <QStringView>

#include "TextCursor.h"

export module Text.TextRange;

import Text.CheckingTextCursor;

export class TextRange
{
public:
    using List = std::vector<TextRange>;

    TextRange(QStringView view, int left, int right)
        : mString(view)
        , mLeftCursor(view, left)
        , mRightCursor(view, right)
    {
        assert(left <= right);
        assert(left >= 0);
        assert(right >= 0);
    }

    const TextCursor& left() const { return mLeftCursor; }
    const TextCursor& right() const { return mRightCursor; }

    int  size() const { return mRightCursor.pos() - mLeftCursor.pos(); }
    bool isEmpty() const { return mLeftCursor.pos() == mRightCursor.pos(); }

    // Returns string without range & cut position
    std::pair<QString, int> cutted() const
    {
        auto ret = mString.toString();
        ret.remove(mLeftCursor.pos(), size());
        return {ret, mLeftCursor.pos()};
    }

    QStringView rangeView() const { return mString.mid(left().pos(), size()); }

    static TextRange selectWord(QStringView str, int startPos)
    {
        auto lWord = CheckingTextCursor::leftWordBorder(str, startPos, Direction::Left);
        auto rWord = CheckingTextCursor::rightWordBorder(str, startPos, Direction::Right);

        if (lWord.check() && rWord.check()) {
            return {str, lWord.pos(), rWord.pos()};
        }

        return {str, startPos, startPos};
    }

protected: // Members
    const QStringView mString;
    const TextCursor  mLeftCursor;
    const TextCursor  mRightCursor;
};
