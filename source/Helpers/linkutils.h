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

#include <functional>

#include <QChar>
#include <QString>
#include <QStringList>
#include <QStringView>

#include "source/Helpers/handytypes.h"
#include "source/Helpers/strrange.h"
#include "source/Helpers/text/textcursor.h"
#include "source/Helpers/text/textrange.h"

// Static class
class LinkUtils
{
public:
    using Cursor        = int;
    using CharCondition = std::function<bool(const QChar&)>;

    using SearchDirection = TextCursor::Direction;

    // Tags
    static bool isInsideTag(QStringView str, Cursor cursor);

    static QString addTag(QString str, Cursor cursor);
    static QString expandTagRight(QString str, Cursor cursor);
    static QString removeTag(QString str, Cursor cursor);

    static StrRange::List extractTagRanges(QStringView str);
    static QString        replaceTags(QString str, const QString& leftReplacer, const QString& rightReplacer);

    static bool tagLengthSuitTerm(const QString& tag, const QString& termName);

    static int getLevDistance(QStringView src, QStringView dst, int limit = 100000);

    static opt<int> getDistanceBetweenTagAndTerm(const QString& tag, const QString& termName, int maxLimit);

    // Words
    static TextRange findWordBorders(QStringView str, Cursor from);
    static int       wordsCount(const QString& string);

    // Brackets
    static bool isPairedBrackets(QStringView str);

    static QChar getBracket(QStringView str, Cursor from, SearchDirection direction);
    static int   getMaxBracketsDepth(QStringView str);

    // Cursor
    static Cursor findCursor(QStringView str, Cursor from, SearchDirection direction, CharCondition exitCondition);
    static opt<StrRange> findBracketsPair(QStringView str, Cursor from);

    static bool isRangeOnBrackets(QStringView str, StrRange range);

    // Exit conditions
    static bool isBracket(const QChar& ch);
    static bool isLeftBracket(const QChar& ch);
    static bool isRightBracket(const QChar& ch);
    static bool isLetterOrNumber(const QChar& ch);
    static bool isLetterOrNumberInverse(const QChar& ch);
    static bool isSpaceCharacter(const QChar& ch);

    constexpr static int  nullCursor   = -1;
    constexpr static auto leftBracket  = '{';
    constexpr static auto rightBracket = '}';

private:
    constexpr static auto emptyBrackets = "{}";
};
