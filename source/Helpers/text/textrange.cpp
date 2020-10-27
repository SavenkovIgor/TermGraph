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

#include "source/Helpers/text/textrange.h"
#include "source/Helpers/text/textsearcher.h"

#include "source/Helpers/text/chartools.h"

int TextRange::leftPos() const { return mLeftCursor.pos(); }

int TextRange::rightPos() const { return mRightCursor.pos(); }

int TextRange::size() const { return mRightCursor.pos() - mLeftCursor.pos(); }

bool TextRange::isEmpty() const { return leftPos() == rightPos(); }

TextRange TextRange::selectWord(QStringView str, int startPos)
{
    auto left  = TextCursor(str, startPos);
    auto right = TextCursor(str, startPos);

    left.moveLeft(CharTools::isLetterOrNumberInverse);
    right.moveRight(CharTools::isLetterOrNumberInverse);

    return TextRange(str, left, right);
}

opt<TextRange> TextRange::selectLink(QStringView str, int startPos)
{
    auto left = TextCursor(str, startPos);

    if (!left.move(Direction::Left, CharTools::isLeftBracketOnRight))
        return std::nullopt;

    // Second bracket must be righter than first, so +1
    auto right = TextCursor(str, startPos + 1);
    if (!right.move(Direction::Right, CharTools::isRightBracketOnLeft))
        return std::nullopt;

    return TextRange(str, left, right);
}

TextRange::TextRange(QStringView view, int left, int right)
    : mString(view)
    , mLeftCursor(view, left)
    , mRightCursor(view, right)
{}

TextRange::TextRange(QStringView view, TextCursor left, TextCursor right)
    : mString(view)
    , mLeftCursor(left)
    , mRightCursor(right)
{}
