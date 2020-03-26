/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
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

#include "source/Helpers/tagutils.h"

#include "source/Helpers/intmatrix.h"

bool TagUtils::isBracket(const QChar& ch)
{
    return ch == leftBracket || ch == rightBracket;
}

bool TagUtils::isLeftBracket(const QChar& ch)
{
    return ch == leftBracket;
}

bool TagUtils::isRightBracket(const QChar& ch)
{
    return ch == rightBracket;
}

bool TagUtils::isLetterOrNumber(const QChar& ch)
{
    return ch.isLetterOrNumber();
}

bool TagUtils::isLetterOrNumberInverse(const QChar& ch)
{
    return !isLetterOrNumber(ch);
}

bool TagUtils::isSpaceCharacter(const QChar& ch)
{
    return ch.isSpace();
}

bool TagUtils::isPairedBrackets(QStringView str)
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

bool TagUtils::isInsideTag(QStringView str, TagUtils::Cursor cursor)
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
QString TagUtils::addTag(QString str, Cursor cursor)
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

QString TagUtils::expandTagRight(QString str, TagUtils::Cursor cursor)
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

QString TagUtils::removeTag(QString str, TagUtils::Cursor cursor)
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

QString TagUtils::replaceTags(QString str, const QString& leftReplacer, const QString& rightReplacer)
{
    if (!isPairedBrackets(str))
        return str;

    str.replace(leftBracket, leftReplacer);
    str.replace(rightBracket, rightReplacer);
    return str;
}

bool TagUtils::tagLengthSuitTerm(const QString& tag, const QString& termName)
{
    int wordsCountInTag = wordsCount(tag);
    int maxWordDistance = 4 * wordsCountInTag;  // Magic numbers. Would be replaced further

    auto stringSizeDiffer = std::abs(termName.size() - tag.size());
    // No need to check. Term is not suit already
    return stringSizeDiffer < maxWordDistance;
}

int TagUtils::getLevDistance(QStringView src, QStringView dst, int limit)
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
        min_in_row = 100000;
        for (int j = 1; j <= n; ++j) {
            cost          = src[i - 1] == dst[j - 1] ? 0 : 1;
            above_cell    = matrix[i - 1][j];
            left_cell     = matrix[i][j - 1];
            diagonal_cell = matrix[i - 1][j - 1];
            matrix[i][j]  = std::min(std::min(above_cell + 1, left_cell + 1), diagonal_cell + cost);
            min_in_row    = std::min(min_in_row, matrix[i][j]);
        }
        if (min_in_row > limit) {
            return 100000;
        }
    }

    return matrix[m][n];
}

std::optional<int> TagUtils::getDistanceBetweenTagAndTerm(const QString& tag, const QString& termName, int maxLimit)
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
    if (stringSizeDiffer > maxWordDistance)  // No need to check. Term is not suit already
        return std::nullopt;

    auto distance = getLevDistance(termName, tag, maxWordDistance);
    if (distance <= maxWordDistance) {
        if (termName.left(3) == tag.left(3)) {
            return distance;
        }
    }

    return std::nullopt;
}

QStringList TagUtils::extractTags(QStringView str)
{
    // На данном этапе считаем, что экранировать символы тегов нельзя
    // Функция работает только с корректными тегами
    // Если в строке непарные скобки или вложенность становится отрицательной
    // сразу возвращаем пустой список
    // То же самое для глубины вложенности скобок больше 1.
    // С такими вариантами тоже не работаем

    if (!isPairedBrackets(str))
        return {};

    if (getBracketsDepth(str) != 1)
        return {};

    QStringList tags;
    QString     wordBuffer;
    bool        insideTag = false;

    for (QChar symbol : str) {
        if (insideTag) {
            if (symbol == rightBracket) {  // Тег кончился - заносим в список
                insideTag = false;
                tags << wordBuffer.simplified();
                wordBuffer.clear();
                continue;
            }
            wordBuffer += symbol;
        }
        if (symbol == leftBracket) {  // Заходим в тег
            insideTag = true;
        }
    }

    tags.removeDuplicates();

    return tags;
}

TagUtils::Cursor TagUtils::findWordBorder(QStringView str, TagUtils::Cursor from, TagUtils::SearchDirection dir)
{
    auto cursor = findCursor(str, from, dir, isLetterOrNumberInverse);

    if (cursor == nullCursor) {
        switch (dir) {
        case SearchDirection::left:
            return 0;
        case SearchDirection::right:
            return str.size();
        }
    }

    return cursor;
}

int TagUtils::wordsCount(const QString& string)
{
    return string.count(' ') + 1;
}

QChar TagUtils::getBracket(QStringView str, TagUtils::Cursor from, TagUtils::SearchDirection direction)
{
    auto cursor = findCursor(str, from, direction, isBracket);

    if (cursor == nullCursor)
        return {};

    switch (direction) {
    case SearchDirection::left:
        return str[cursor - 1];
    case SearchDirection::right:
        return str[cursor];
    }

    return {};
}

int TagUtils::getBracketsDepth(QStringView str)
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

bool TagUtils::isValidCursor(QStringView str, TagUtils::Cursor cursor)
{
    // Cursor can be after last symbol, and it's correct
    return 0 <= cursor && cursor <= str.size();
}

TagUtils::Cursor TagUtils::findCursor(QStringView     str,
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

TagUtils::Cursor TagUtils::leftSearch(QStringView str, Cursor cursor, CharCondition exitCondition)
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

TagUtils::Cursor TagUtils::rightSearch(QStringView str, Cursor cursor, CharCondition exitCondition)
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
