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

#include "source/Helpers/validators/linktextvalidator.h"

#include "source/Helpers/text/chartools.h"

bool LinkTextValidator::isValidLinkString(QStringView linkString)
{
    if (!isPairedBrackets(linkString))
        return false;

    if (bracketsMaxDepth(linkString) > 1)
        return false;

    return true;
}

bool LinkTextValidator::isPairedBrackets(QStringView str)
{
    int depth = 0;

    for (auto sym : str) {
        if (sym == CharTools::leftBracket) {
            depth++;
        } else if (sym == CharTools::rightBracket) {
            depth--;
        }

        if (depth < 0)
            return false;
    }

    return depth == 0;
}

int LinkTextValidator::bracketsMaxDepth(QStringView str)
{
    // Check before call!
    assert(isPairedBrackets(str));

    int depth    = 0;
    int maxDepth = 0;

    for (auto sym : str) {
        if (sym == CharTools::leftBracket) {
            depth++;
        } else if (sym == CharTools::rightBracket) {
            depth--;
        }
        maxDepth = std::max(maxDepth, depth);
    }

    return maxDepth;
}
