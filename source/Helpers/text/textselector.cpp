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

#include "source/Helpers/text/textselector.h"

#include "source/Helpers/text/checkingtextcursor.h"

TextRange TextSelector::selectWord(QStringView str, int startPos)
{
    auto cursor = TextCursor(str, startPos);

    auto lWord = CheckingTextCursor::leftWordBorder(str, startPos, Direction::Left);
    auto rWord = CheckingTextCursor::rightWordBorder(str, startPos, Direction::Right);

    if (lWord.check() && rWord.check())
        return TextRange(str, lWord.pos(), rWord.pos());

    return TextRange(str, startPos, startPos);
}

opt<TextRange> TextSelector::selectLink(QStringView str, int startPos)
{
    if (!TextCursor::isValidCursor(str, startPos))
        return std::nullopt;

    auto cursor = TextCursor(str, startPos);

    auto lBracket = CheckingTextCursor::anyBracketOnLeft(str, startPos, Direction::Left);
    auto rBracket = CheckingTextCursor::anyBracketOnRight(str, startPos, Direction::Right);

    if (!lBracket.check() || !rBracket.check())
        return std::nullopt;

    lBracket--;
    rBracket++;

    if (lBracket.right() == CharTools::leftBracket && rBracket.left() == CharTools::rightBracket)
        return TextRange(str, lBracket.pos(), rBracket.pos());

    return std::nullopt;
}
