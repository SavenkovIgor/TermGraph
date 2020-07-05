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

#include "source/Helpers/linkutils.h"

#include <limits>

#include "source/Helpers/intmatrix.h"

bool LinkUtils::isBracket(const QChar& ch) { return ch == leftBracket || ch == rightBracket; }

bool LinkUtils::isLeftBracket(const QChar& ch) { return ch == leftBracket; }

bool LinkUtils::isRightBracket(const QChar& ch) { return ch == rightBracket; }

bool LinkUtils::isLetterOrNumber(const QChar& ch) { return ch.isLetterOrNumber(); }

bool LinkUtils::isLetterOrNumberInverse(const QChar& ch) { return !isLetterOrNumber(ch); }

bool LinkUtils::isSpaceCharacter(const QChar& ch) { return ch.isSpace(); }

bool LinkUtils::isPairedBrackets(QStringView str)
{
    int depth = 0;

    for (auto sym : str) {
        if (sym == leftBracket) {
            depth++;
        } else if (sym == rightBracket) {
            depth--;
        }

        if (depth < 0)
            return false;
    }

    return depth == 0;
}

bool LinkUtils::isInsideTag(QStringView str, LinkUtils::Cursor cursor)
{
    if (!isValidCursor(str, cursor))
        return false;

    QChar firstLeftBracket  = getBracket(str, cursor, SearchDirection::left);
    QChar firstRightBracket = getBracket(str, cursor, SearchDirection::right);

    return firstLeftBracket == leftBracket && firstRightBracket == rightBracket;
}

/// Описание:
/// Функция в пустой строке или строке с пробелами вернет {}
/// Функция на границе строки обрамит крайнее слово
/// При встрече уже обрамленного тега - ничего не сделает
QString LinkUtils::addTag(QString str, Cursor cursor)
{
    // Проверка корректности курсора
    if (!isValidCursor(str, cursor))
        return str;

    // Нельзя просто так упрощать строку - индекс курсора же не меняется
    // str = str.simplified();

    // Если мы уже находимся внутри тега - ничего не делаем
    if (isInsideTag(str, cursor))
        return str;

    int leftWordBorder  = findWordBorder(str, cursor, SearchDirection::left);
    int rightWordBorder = findWordBorder(str, cursor, SearchDirection::right);

    //Сначала вставляем правую, потом левую из за смещения курсора
    str.insert(rightWordBorder, rightBracket);
    str.insert(leftWordBorder, leftBracket);

    return str;
}

QString LinkUtils::expandTagRight(QString str, LinkUtils::Cursor cursor)
{
    if (!isValidCursor(str, cursor))
        return str;

    if (!isInsideTag(str, cursor))
        return str;

    // Move to right bracket
    auto bracketPos = findCursor(str, cursor, SearchDirection::right, isBracket);
    // Move to word
    auto wordStartPos   = findCursor(str, bracketPos + 1, SearchDirection::right, isLetterOrNumber);
    auto openBracketPos = findCursor(str, bracketPos + 1, SearchDirection::right, isLeftBracket);

    // Protection from capturing next tag
    if (openBracketPos != nullCursor) {
        if (wordStartPos > openBracketPos) {
            return str;
        }
    }

    auto wordEndPos = findCursor(str, wordStartPos, SearchDirection::right, isLetterOrNumberInverse);

    if (wordStartPos != nullCursor && wordEndPos == nullCursor) {
        wordEndPos = str.size();
    }

    if (bracketPos != nullCursor && wordEndPos != nullCursor) {
        str.insert(wordEndPos, rightBracket);
        str.remove(bracketPos, 1);
        return str;
    }

    return str;
}

QString LinkUtils::removeTag(QString str, LinkUtils::Cursor cursor)
{
    // Проверка корректности курсора
    if (!isValidCursor(str, cursor))
        return str;

    // Нельзя просто так упрощать строку - индекс курсора же не меняется
    // str = str.simplified();

    // Если мы не находимся внутри тега - ничего не делаем
    if (!isInsideTag(str, cursor))
        return str;

    auto leftBracketPos  = findCursor(str, cursor, SearchDirection::left, isBracket);
    auto rightBracketPos = findCursor(str, cursor, SearchDirection::right, isBracket);

    //Сначала удаляем правую, потом левую из за смещения индексов
    str.remove(rightBracketPos, 1);
    str.remove(leftBracketPos - 1, 1);

    return str;
}

QString LinkUtils::replaceTags(QString str, const QString& leftReplacer, const QString& rightReplacer)
{
    if (!isPairedBrackets(str))
        return str;

    str.replace(leftBracket, leftReplacer);
    str.replace(rightBracket, rightReplacer);
    return str;
}

bool LinkUtils::tagLengthSuitTerm(const QString& tag, const QString& termName)
{
    int wordsCountInTag = wordsCount(tag);
    int maxWordDistance = 4 * wordsCountInTag; // Magic numbers. Would be replaced further

    auto stringSizeDiffer = std::abs(termName.size() - tag.size());
    // No need to check. Term is not suit already
    return stringSizeDiffer <= maxWordDistance;
}

int LinkUtils::getLevDistance(QStringView src, QStringView dst, int limit)
{
    const int m = src.size();
    const int n = dst.size();

    if (m == 0)
        return n;

    if (n == 0)
        return m;

    static IntMatrix matrixContainer;
    matrixContainer.checkSize(n + 1, m + 1);

    auto& matrix = matrixContainer.matrix;

    for (int i = 0; i <= m; ++i) {
        matrix[i][0] = i;
    }

    for (int i = 0; i <= n; ++i) {
        matrix[0][i] = i;
    }

    int above_cell;
    int left_cell;
    int diagonal_cell;
    int cost;
    int min_in_row;

    for (int i = 1; i <= m; ++i) {
        min_in_row = std::numeric_limits<int>::max();
        for (int j = 1; j <= n; ++j) {
            cost          = src[i - 1] == dst[j - 1] ? 0 : 1;
            above_cell    = matrix[i - 1][j];
            left_cell     = matrix[i][j - 1];
            diagonal_cell = matrix[i - 1][j - 1];
            matrix[i][j]  = std::min(std::min(above_cell + 1, left_cell + 1), diagonal_cell + cost);
            min_in_row    = std::min(min_in_row, matrix[i][j]);
        }
        if (min_in_row > limit) {
            return std::numeric_limits<int>::max();
        }
    }

    return matrix[m][n];
}

opt<int> LinkUtils::getDistanceBetweenTagAndTerm(const QString& tag, const QString& termName, int maxLimit)
{
    // For developing needs
    assert(termName == termName.toLower());
    assert(tag == tag.toLower());

    // Exact match
    if (termName.size() == tag.size() && termName == tag)
        return 0;

    int wordsCountInTag = tag.count(' ') + 1;
    int maxWordDistance = std::min(4 * wordsCountInTag, maxLimit);

    auto stringSizeDiffer = std::abs(termName.size() - tag.size());
    if (stringSizeDiffer > maxWordDistance) // No need to check. Term is not suit already
        return std::nullopt;

    auto distance = getLevDistance(termName, tag, maxWordDistance);
    if (distance <= maxWordDistance) {
        if (termName.left(3) == tag.left(3)) {
            return distance;
        }
    }

    return std::nullopt;
}

StrRange::List LinkUtils::extractTagRanges(QStringView str)
{
    Cursor         from = 0;
    StrRange::List ret;

    while (true) {
        auto optRange = findBracketsPair(str, from);
        if (optRange) {
            from = optRange.value().right();
            ret.push_back(optRange.value());
        } else {
            break;
        }
    }

    return ret;
}

LinkUtils::Cursor LinkUtils::findWordBorder(QStringView str, LinkUtils::Cursor from, LinkUtils::SearchDirection dir)
{
    auto cursor = findCursor(str, from, dir, isLetterOrNumberInverse);

    if (cursor == nullCursor) {
        switch (dir) {
        case SearchDirection::left: return 0;
        case SearchDirection::right: return str.size();
        }
    }

    return cursor;
}

int LinkUtils::wordsCount(const QString& string)
{
    if (!string.isEmpty())
        return string.simplified().count(' ') + 1;

    return 0;
}

QChar LinkUtils::getBracket(QStringView str, LinkUtils::Cursor from, LinkUtils::SearchDirection direction)
{
    auto cursor = findCursor(str, from, direction, isBracket);

    if (cursor == nullCursor)
        return {};

    switch (direction) {
    case SearchDirection::left: return str[cursor - 1];
    case SearchDirection::right: return str[cursor];
    }

    return {};
}

int LinkUtils::getMaxBracketsDepth(QStringView str)
{
    if (!isPairedBrackets(str))
        return -1;

    int depth    = 0;
    int maxDepth = 0;

    for (auto sym : str) {
        if (sym == leftBracket) {
            depth++;
        } else if (sym == rightBracket) {
            depth--;
        }
        maxDepth = std::max(maxDepth, depth);
    }

    return maxDepth;
}

bool LinkUtils::isValidCursor(QStringView str, LinkUtils::Cursor cursor)
{
    // Cursor can be after last symbol, and it's correct
    return 0 <= cursor && cursor <= str.size();
}

LinkUtils::Cursor LinkUtils::findCursor(QStringView     str,
                                        Cursor          from,
                                        SearchDirection direction,
                                        CharCondition   exitCondition)
{
    if (direction == SearchDirection::left) {
        return leftSearch(str, from, exitCondition);
    } else {
        return rightSearch(str, from, exitCondition);
    }
}

opt<StrRange> LinkUtils::findBracketsPair(QStringView str, LinkUtils::Cursor from)
{
    auto leftBracket = rightSearch(str, from, &isLeftBracket);
    if (leftBracket == nullCursor)
        return std::nullopt;

    auto rightBracket = rightSearch(str, leftBracket, &isRightBracket);
    if (rightBracket == nullCursor)
        return std::nullopt;

    assert(leftBracket != rightBracket);
    assert(str[leftBracket] == LinkUtils::leftBracket);
    assert(str[rightBracket] == LinkUtils::rightBracket);

    return StrRange::fromLeftRight(leftBracket, rightBracket);
}

LinkUtils::Cursor LinkUtils::leftSearch(QStringView str, Cursor cursor, CharCondition exitCondition)
{
    // Если мы у левой границы строки,
    // значит условие выхода не выполнилось - возвращаем -1
    if (cursor <= 0 || cursor == nullCursor)
        return nullCursor;

    const QChar leftChar = str[cursor - 1];

    if (exitCondition(leftChar)) {
        // Если для символа слева условие выполняется, возвращаем эту позицию
        return cursor;
    } else {
        // Иначе ищем левее
        return leftSearch(str, cursor - 1, exitCondition);
    }
}

LinkUtils::Cursor LinkUtils::rightSearch(QStringView str, Cursor cursor, CharCondition exitCondition)
{
    // Если мы у правой границы строки,
    // значит условие выхода не выполнилось - возвращаем -1
    if (cursor >= str.size() || cursor == nullCursor)
        return nullCursor;

    const QChar rightChar = str[cursor];

    if (exitCondition(rightChar)) {
        // Если для символа справа условие выполняется, возвращаем эту позицию
        return cursor;
    } else {
        // Иначе ищем правее
        return rightSearch(str, cursor + 1, exitCondition);
    }
}

bool LinkUtils::isRangeOnBrackets(QStringView str, StrRange range)
{
    return str[range.left()] == leftBracket && str[range.right()] == rightBracket;
}
