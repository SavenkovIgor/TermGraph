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

#include <vector>

#include <QStringView>

#include "source/helpers/text/textcursor.h"

class TextRange
{
public:
    using List = std::vector<TextRange>;

    TextRange(QStringView view, int left, int right);

    const TextCursor& left() const;
    const TextCursor& right() const;

    int  size() const;
    bool isEmpty() const;

    // Returns string without range & cut position
    std::pair<QString, int> cutted() const;

    QStringView rangeView() const;

    static TextRange selectWord(QStringView str, int startPos);

protected: // Members
    const QStringView mString;
    const TextCursor  mLeftCursor;
    const TextCursor  mRightCursor;
};