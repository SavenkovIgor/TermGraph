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
#include "source/Helpers/text/textcursor.h"
#include "source/Helpers/text/textsearcher.h"
#include "source/Helpers/validators/linktextvalidator.h"

bool LinkUtils::isInsideTag(QStringView str, int cursor)
{
    if (!TextCursor::isValidCursor(str, cursor))
        return false;

    auto textCursor = TextCursor(str, cursor);

    auto firstLeftBracket  = TextSearcher::find(textCursor, Direction::Left, CharTools::isBracket);
    auto firstRightBracket = TextSearcher::find(textCursor, Direction::Right, CharTools::isBracket);

    if (!firstLeftBracket.has_value() || !firstRightBracket.has_value())
        return false;

    auto leftChar  = firstLeftBracket->left().value_or(QChar());
    auto rightChar = firstRightBracket->right().value_or(QChar());

    return leftChar == CharTools::leftBracket && rightChar == CharTools::rightBracket;
}

/// Описание:
/// Функция в пустой строке или строке с пробелами вернет {}
/// Функция на границе строки обрамит крайнее слово
/// При встрече уже обрамленного тега - ничего не сделает
QString LinkUtils::addTag(QString str, int cursor)
{
    // Проверка корректности курсора
    if (!TextCursor::isValidCursor(str, cursor))
        return str;

    // Нельзя просто так упрощать строку - индекс курсора же не меняется
    // str = str.simplified();

    // Если мы уже находимся внутри тега - ничего не делаем
    if (isInsideTag(str, cursor))
        return str;

    auto word = TextSearcher::selectWord(str, cursor);

    //Сначала вставляем правую, потом левую из за смещения курсора
    str.insert(word.right().pos(), CharTools::rightBracket);
    str.insert(word.left().pos(), CharTools::leftBracket);

    return str;
}

QString LinkUtils::expandTagRight(QString str, int cursor)
{
    if (!TextCursor::isValidCursor(str, cursor))
        return str;

    if (!isInsideTag(str, cursor))
        return str;

    // Move to right bracket
    auto bracketPos = findCursor(str, cursor, Direction::Right, CharTools::isBracket);
    // Move to word
    auto wordStartPos = findCursor(str, bracketPos + 1, Direction::Right, CharTools::isLetterOrNumber);
    if (wordStartPos == -1)
        return str;

    auto openBracketPos = findCursor(str, bracketPos + 1, Direction::Right, CharTools::isLeftBracket);

    // Protection from capturing next tag
    if (openBracketPos != nullCursor) {
        if (wordStartPos > openBracketPos) {
            return str;
        }
    }

    auto wordEndPos = findCursor(str, wordStartPos, Direction::Right, CharTools::notLetterOrNumber);

    if (wordStartPos != nullCursor && wordEndPos == nullCursor) {
        wordEndPos = str.size();
    }

    if (bracketPos != nullCursor && wordEndPos != nullCursor) {
        str.insert(wordEndPos, CharTools::rightBracket);
        str.remove(bracketPos, 1);
        return str;
    }

    return str;
}

QString LinkUtils::removeTag(QString str, int cursor)
{
    // Проверка корректности курсора
    if (!TextCursor::isValidCursor(str, cursor))
        return str;

    // Нельзя просто так упрощать строку - индекс курсора же не меняется
    // str = str.simplified();

    // Если мы не находимся внутри тега - ничего не делаем
    if (!isInsideTag(str, cursor))
        return str;

    auto textCursor   = TextCursor(str, cursor);
    auto leftBracket  = TextSearcher::find(textCursor, Direction::Left, CharTools::isBracket).value();
    auto rightBracket = TextSearcher::find(textCursor, Direction::Right, CharTools::isBracket).value();

    //Сначала удаляем правую, потом левую из за смещения индексов
    str.remove(rightBracket.pos(), 1);
    str.remove(leftBracket.pos() - 1, 1);

    return str;
}

QString LinkUtils::replaceTags(QString str, const QString& leftReplacer, const QString& rightReplacer)
{
    if (!LinkTextValidator::isPairedBrackets(str))
        return str;

    str.replace(CharTools::leftBracket, leftReplacer);
    str.replace(CharTools::rightBracket, rightReplacer);
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

TextRange::List LinkUtils::extractLinkRanges(QStringView str)
{
    TextRange::List ret;

    if (!LinkTextValidator::isValidLinkString(str))
        return ret;

    int count = linksCount(str);

    for (int i = 0; i < count; i++)
        ret.push_back(linkAt(str, i));

    return ret;
}

int LinkUtils::wordsCount(const QString& string)
{
    if (!string.isEmpty())
        return string.simplified().count(' ') + 1;

    return 0;
}

int LinkUtils::findCursor(QStringView str, int from, Direction direction, CharTools::ShortCondition exitCondition)
{
    auto cursor = TextSearcher::find(TextCursor(str, from), direction, exitCondition);
    return cursor.has_value() ? cursor.value().pos() : nullCursor;
}

int LinkUtils::linksCount(QStringView str)
{
    bool isValidLinks = LinkTextValidator::isValidLinkString(str);

    assert(isValidLinks);
    if (!isValidLinks)
        return 0;

    return str.toString().count(CharTools::leftBracket);
}

TextRange LinkUtils::linkAt(QStringView str, int index)
{
    int strPos = -1;

    for (int i = 0; i <= index; i++)
        strPos = str.indexOf(CharTools::leftBracket, strPos + 1);

    auto ret = TextSearcher::selectLink(str, strPos + 1);

    assert(ret.has_value());

    return ret.value();
}

bool LinkUtils::isRangeOnLink(TextRange range)
{
    return range.left().right() == CharTools::leftBracket && range.right().left() == CharTools::rightBracket;
}
