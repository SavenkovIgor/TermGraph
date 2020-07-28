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

TextCursor::TextCursor(QStringView strView, int pos)
    : mPos(pos)
    , mString(strView)
{
    assert(isValidCursor(mString, pos));
}

opt<TextCursor> TextCursor::tryCreateCursor(QStringView view, int pos)
{
    if (!isValidCursor(view, pos))
        return std::nullopt;

    return TextCursor(view, pos);
}

bool TextCursor::moveLeft()
{
    assert(isValidCursor());
    if (canMoveLeft()) {
        mPos--;
        return true;
    }
    return false;
}

bool TextCursor::moveRight()
{
    assert(isValidCursor());
    if (canMoveRight()) {
        mPos++;
        return true;
    }

    return false;
}

bool TextCursor::moveLeft(TextCursor::Condition whileCond)
{
    do {
        auto character = left();
        if (!character)
            return false;

        if (whileCond(character.value()))
            return true;

    } while (moveLeft());

    return false;
}

bool TextCursor::moveRight(TextCursor::Condition whileCond)
{
    do {
        auto character = right();
        if (!character)
            return false;

        if (whileCond(character.value()))
            return true;

    } while (moveRight());

    return false;
}

bool TextCursor::moveLeft(const QChar &stopChar)
{
    auto cond = [stopChar](const QChar &ch) { return ch == stopChar; };
    return moveLeft(cond);
}

bool TextCursor::moveRight(const QChar &stopChar)
{
    auto cond = [stopChar](const QChar &ch) { return ch == stopChar; };
    return moveRight(cond);
}

bool TextCursor::move(TextCursor::Direction dir, TextCursor::Condition whileCond)
{
    if (dir == Direction::Right)
        return moveRight(whileCond);

    return moveLeft(whileCond);
}

bool TextCursor::move(TextCursor::Direction dir, const QChar &stopChar)
{
    auto cond = [stopChar](const QChar &ch) { return ch == stopChar; };
    return move(dir, cond);
}

bool TextCursor::canMoveLeft() const { return mPos > 0; }

bool TextCursor::canMoveRight() const { return mPos < mString.size(); }

int TextCursor::pos() const { return mPos; }

opt<QChar> TextCursor::left() const
{
    assert(isValidCursor());
    if (!canMoveLeft()) // On left border
        return std::nullopt;

    return mString[mPos - 1];
}

opt<QChar> TextCursor::right() const
{
    assert(isValidCursor());
    if (!canMoveRight()) // On right border
        return std::nullopt;

    return mString[mPos];
}
