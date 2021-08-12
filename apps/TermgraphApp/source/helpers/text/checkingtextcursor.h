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

#include "source/helpers/text/chartools.h"
#include "source/helpers/text/textcursor.h"

class CheckingTextCursor : public TextCursor
{
public:
    using Condition = CharTools::ShortCondition;

    CheckingTextCursor(QStringView    strView,
                       int            pos,
                       Condition      leftChecker,
                       Condition      rightChecker,
                       opt<Direction> autoStartSearch = std::nullopt);

    bool check() const;

    bool search(Direction dir);

    static CheckingTextCursor rightWordBorder(QStringView    strView,
                                              int            pos             = 0,
                                              opt<Direction> autoStartSearch = std::nullopt);

    static CheckingTextCursor leftWordBorder(QStringView    strView,
                                             int            pos             = 0,
                                             opt<Direction> autoStartSearch = std::nullopt);

    static CheckingTextCursor leftBracketOnRight(QStringView    strView,
                                                 int            pos             = 0,
                                                 opt<Direction> autoStartSearch = std::nullopt);

    static CheckingTextCursor rightBracketOnLeft(QStringView    strView,
                                                 int            pos             = 0,
                                                 opt<Direction> autoStartSearch = std::nullopt);

    static CheckingTextCursor anyBracketOnRight(QStringView    strView,
                                                int            pos             = 0,
                                                opt<Direction> autoStartSearch = std::nullopt);

    static CheckingTextCursor anyBracketOnLeft(QStringView    strView,
                                               int            pos             = 0,
                                               opt<Direction> autoStartSearch = std::nullopt);

private:
    const Condition mLeftChecker;
    const Condition mRightChecker;
};
