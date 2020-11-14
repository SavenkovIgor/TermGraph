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

#include "source/Helpers/link/linkutils.h"

#include <limits>

#include "source/Helpers/intmatrix.h"
#include "source/Helpers/text/chartools.h"
#include "source/Helpers/text/checkingtextcursor.h"
#include "source/Helpers/text/textcursor.h"
#include "source/Helpers/validators/linktextvalidator.h"

bool LinkUtils::isInsideTag(QStringView str, int cursor) { return TextLink::selectLink(str, cursor).has_value(); }

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

    auto word = TextRange::selectWord(str, cursor);

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
    auto rBracket = CheckingTextCursor::anyBracketOnRight(str, cursor, Direction::Right);

    assert(rBracket.right() == CharTools::rightBracket);

    auto lWord = CheckingTextCursor::leftWordBorder(str, rBracket.pos() + 1, Direction::Right);

    if (!lWord.check()) // if not found
        return str;

    auto lBracketMaybe = CheckingTextCursor::leftBracketOnRight(str, rBracket.pos() + 1, Direction::Right);

    // Protection from capturing next tag
    if (lBracketMaybe.check()) {
        if (lWord.pos() > lBracketMaybe.pos()) {
            return str;
        }
    }

    auto rWord = CheckingTextCursor::rightWordBorder(str, lWord.pos(), Direction::Right);

    if (rBracket.check() && rWord.check()) {
        str.insert(rWord.pos(), CharTools::rightBracket);
        str.remove(rBracket.pos(), 1);
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

    auto leftBracket  = CheckingTextCursor::anyBracketOnLeft(str, cursor, Direction::Left);
    auto rightBracket = CheckingTextCursor::anyBracketOnRight(str, cursor, Direction::Right);

    assert(leftBracket.check());
    assert(rightBracket.check());

    //Сначала удаляем правую, потом левую из за смещения индексов
    str.remove(rightBracket.pos(), 1);
    str.remove(leftBracket.pos() - 1, 1);

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

int LinkUtils::wordsCount(const QString& string)
{
    if (!string.isEmpty())
        return string.simplified().count(' ') + 1;

    return 0;
}

TextLink LinkUtils::linkAt(QStringView str, int index)
{
    int strPos = -1;

    for (int i = 0; i <= index; i++)
        strPos = str.indexOf(CharTools::leftBracket, strPos + 1);

    auto ret = TextLink::selectLink(str, strPos + 1);

    assert(ret.has_value());

    return ret.value();
}
