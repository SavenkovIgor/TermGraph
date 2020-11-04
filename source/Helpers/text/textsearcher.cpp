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

#include "source/Helpers/text/textsearcher.h"

#include "source/Helpers/text/checkingtextcursor.h"

opt<TextCursor> TextSearcher::find(TextCursor startPos, Direction dir, CharTools::ShortCondition checker)
{
    do {
        auto character = startPos.getSymbol(dir);
        if (character.isNull())
            return std::nullopt;

        if (checker(character))
            return startPos;

        startPos.move(dir);
    } while (startPos.canMove(dir));

    return std::nullopt;
}

TextRange TextSearcher::selectWord(QStringView str, int startPos)
{
    auto cursor = TextCursor(str, startPos);

    auto leftOpt  = TextSearcher::find(cursor, Direction::Left, CharTools::notLetterOrNumber);
    auto rightOpt = TextSearcher::find(cursor, Direction::Right, CharTools::notLetterOrNumber);

    auto left  = leftOpt.value_or(TextCursor(str));
    auto right = rightOpt.value_or(TextCursor(str, str.length()));

    return TextRange(str, left.pos(), right.pos());
}

opt<TextRange> TextSearcher::selectLink(QStringView str, int startPos)
{
    if (!TextCursor::isValidCursor(str, startPos))
        return std::nullopt;

    auto cursor = TextCursor(str, startPos);

    auto lBracket = CheckingTextCursor::anyBracketOnLeft(str, startPos);
    auto rBracket = CheckingTextCursor::anyBracketOnRight(str, startPos);

    if (!lBracket.search(Direction::Left) || !rBracket.search(Direction::Right))
        return std::nullopt;

    lBracket--;
    rBracket++;

    if (lBracket.right() == CharTools::leftBracket && rBracket.left() == CharTools::rightBracket)
        return TextRange(str, lBracket.pos(), rBracket.pos());

    return std::nullopt;
}
