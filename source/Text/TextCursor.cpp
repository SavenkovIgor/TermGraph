// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <algorithm>

#include <QStringView>
#include <QtGlobal>

export module Text.TextCursor;

import CommonTools.HandyTypes;

using namespace std;

export enum class Direction { Left, Right };

export class TextCursor
{
public:
    TextCursor(QStringView strView, int pos = 0)
        : mPos(clamp(pos, 0, asInt(strView.size())))
        , mString(strView)
    {
        assert(mPos == pos);
    }

    // Tools for creation safety
    static bool isValidCursor(QStringView view, int cursor) { return 0 <= cursor && cursor <= view.size(); }

    int  pos() const { return mPos; }
    bool atStart() const { return mPos == 0; }
    bool atEnd() const { return mPos == mString.size(); }
    bool atBorder() const { return atStart() || atEnd(); }

    // WARNING! Can return null QChar!
    QChar left() const { return getSymbol(Direction::Left); }
    QChar right() const { return getSymbol(Direction::Right); }

    QChar getSymbol(Direction dir) const
    {
        if (!canMove(dir)) { // Near border
            return {};
        }

        int offset = dir == Direction::Left ? -1 : 0;
        return mString[mPos + offset];
    }

    bool canMove(Direction dir) const { return dir == Direction::Left ? !atStart() : !atEnd(); }

    bool move(Direction dir)
    {
        if (canMove(dir)) {
            int offset = dir == Direction::Left ? -1 : 1;
            mPos += offset;
            return true;
        }
        return false;
    }

    TextCursor &operator++(int)
    {
        move(Direction::Right);
        return *this;
    }

    TextCursor &operator--(int)
    {
        move(Direction::Left);
        return *this;
    }

private:
    int mPos = 0;

    const QStringView mString;
};
