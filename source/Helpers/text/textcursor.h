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

#pragma once

#include <QStringView>
#include <QtGlobal>

#include "source/Helpers/handytypes.h"

class TextCursor
{
public:
    using Condition     = std::function<bool(const QChar&)>;
    using FullCondition = std::function<bool(const opt<QChar>, const opt<QChar>)>;

    TextCursor(QStringView strView, int pos);

    // static tools
    inline static bool     isValidCursor(QStringView view, int cursor) { return 0 <= cursor && cursor <= view.size(); }
    static opt<TextCursor> create(QStringView view, int pos);
    static opt<TextCursor> find(QStringView inString, int from, Direction dir, Condition matchCondition);

    bool move(Direction dir, Condition whileCond);
    bool move(Direction dir, FullCondition fullCond);
    bool moveLeft(Condition whileCond);
    bool moveRight(Condition whileCond);

    bool move(Direction dir, const QChar& stopChar);

    opt<QChar> lrChar(Direction dir) const;

    int pos() const;

    bool       canMove(Direction dir) const;
    opt<QChar> left() const;
    opt<QChar> right() const;
    bool       move(Direction dir);

    TextCursor& operator++(int);
    TextCursor& operator--(int);

private: // Methods
    // Cursor can be after last symbol, and it's correct
    inline bool isValidCursor() const { return isValidCursor(mString, mPos); }

private: // Members
    int mPos = 0;

    const QStringView mString;
};
