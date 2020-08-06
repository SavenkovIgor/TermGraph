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

#include "source/Helpers/text/chartools.h"

bool CharTools::isBracket(const QChar &ch) { return ch == leftBracket || ch == rightBracket; }

bool CharTools::isLeftBracket(const QChar &ch) { return ch == leftBracket; }

bool CharTools::isRightBracket(const QChar &ch) { return ch == rightBracket; }

bool CharTools::isLetterOrNumber(const QChar &ch) { return ch.isLetterOrNumber(); }

bool CharTools::isLetterOrNumberInverse(const QChar &ch) { return !ch.isLetterOrNumber(); }

bool CharTools::isLetterLeftAndNotLetterRight(const opt<QChar> lSym, const opt<QChar> rSym)
{
    if (!lSym.has_value())
        return false;

    if (isLetterOrNumber(lSym.value())) {
        if (!rSym.has_value())
            return true;
        else
            return !isLetterOrNumber(rSym.value());
    }

    return false;
}

bool CharTools::isLeftBracketOnRight([[maybe_unused]] const opt<QChar> lSym, const opt<QChar> rSym)
{
    if (!rSym.has_value())
        return false;
    return rSym.value() == leftBracket;
}

bool CharTools::isRightBracketOnLeft(const opt<QChar> lSym, [[maybe_unused]] const opt<QChar> rSym)
{
    if (!lSym.has_value())
        return false;
    return lSym.value() == rightBracket;
}
