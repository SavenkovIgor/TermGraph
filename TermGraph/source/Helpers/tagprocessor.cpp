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

#include "tagprocessor.h"

TagProcessor::TagProcessor(QObject* parent)
    : QObject(parent)
{}

bool TagProcessor::isBracket(const QChar& ch)
{
    return ch == leftBracket || ch == rightBracket;
}

bool TagProcessor::isLeftBracket(const QChar& ch)
{
    return ch == leftBracket;
}

bool TagProcessor::isRightBracket(const QChar& ch)
{
    return ch == rightBracket;
}

bool TagProcessor::isLetterOrNumber(const QChar& ch)
{
    return ch.isLetterOrNumber();
}

bool TagProcessor::isLetterOrNumberInverse(const QChar& ch)
{
    return !isLetterOrNumber(ch);
}

bool TagProcessor::isSpaceCharacter(const QChar& ch)
{
    return ch.isSpace();
}

int TagProcessor::searchWordBorder(const SearchDirection direction, QStringView str, int cursorPos)
{
    auto pos = getCursorPosition(direction, str, cursorPos, isLetterOrNumberInverse);

    if (pos == -1) {
        switch (direction) {
        case SearchDirection::left:
            return 0;
        case SearchDirection::right:
            return str.size();
        }
    }

    return pos;
}

QChar TagProcessor::getNearesBracket(const SearchDirection direction, QStringView str, int cursorPos)
{
    auto pos = getCursorPosition(direction, str, cursorPos, isBracket);

    if (pos == -1)
        return {};

    switch (direction) {
    case SearchDirection::left:
        return str[pos - 1];
    case SearchDirection::right:
        return str[pos];
    }

    return {};
}

bool TagProcessor::isInsideTag(QStringView str, int cursorPos)
{
    if (!isValidCursor(str, cursorPos))
        return false;

    QChar firstLeftBracket  = getNearesBracket(SearchDirection::left, str, cursorPos);
    QChar firstRightBracket = getNearesBracket(SearchDirection::right, str, cursorPos);

    return firstLeftBracket == leftBracket && firstRightBracket == rightBracket;
}

bool TagProcessor::isPairedBrackets(QStringView str)
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

int TagProcessor::getMaxDepthOfNestedBrackets(QStringView str)
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

int TagProcessor::getLevDistance(QStringView src, QStringView dst, int limit)
{
    const int m = src.size();
    const int n = dst.size();

    if (m == 0)
        return n;

    if (n == 0)
        return m;

    std::vector<std::vector<int>> matrix(m + 1);

    for (int i = 0; i <= m; ++i) {
        matrix[i].resize(n + 1);
        matrix[i][0] = i;
    }

    for (int i = 0; i <= n; ++i) {
        matrix[0][i] = i;
    }

    int above_cell, left_cell, diagonal_cell, cost, min_in_row;

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

int TagProcessor::wordsCount(const QString& string)
{
    return string.count(' ') + 1;
}

bool TagProcessor::tagLengthSuitTerm(const QString& tag, const QString& termName)
{
    int wordsCountInTag = wordsCount(tag);
    int maxWordDistance = 4 * wordsCountInTag;  // Magic numbers. Would be replaced further

    auto stringSizeDiffer = std::abs(termName.size() - tag.size());
    // No need to check. Term is not suit already
    return stringSizeDiffer < maxWordDistance;
}

std::optional<int> TagProcessor::getDistanceBetweenTagAndTerm(const QString& tag, const QString& termName)
{
    // For developing needs
    assert(termName == termName.toLower());
    assert(tag == tag.toLower());

    // Exact match
    if (termName.size() == tag.size() && termName == tag)
        return 0;

    int wordsCountInTag = tag.count(' ') + 1;
    int maxWordDistance = 4 * wordsCountInTag;

    auto stringSizeDiffer = std::abs(termName.size() - tag.size());
    if (stringSizeDiffer > maxWordDistance)  // No need to check. Term is not suit already
        return std::nullopt;

    auto distance = TagProcessor::getLevDistance(termName, tag, maxWordDistance);
    if (distance <= maxWordDistance) {
        if (termName.left(3) == tag.left(3)) {
            return distance;
        }
    }

    return std::nullopt;
}

bool TagProcessor::isValidCursor(const QString& str, int cursorPosition)
{
    return isValidCursor(QStringView(str), cursorPosition);
}

bool TagProcessor::isValidCursor(QStringView str, int cursorPosition)
{
    // Cursor can be after last symbol, and it's correct
    return 0 <= cursorPosition && cursorPosition <= str.size();
}

bool TagProcessor::isInsideTag(const QString& str, int cursorPos)
{
    return isInsideTag(QStringView(str), cursorPos);
}

int TagProcessor::getCursorPosition(const SearchDirection&     direction,
                                    QStringView                str,
                                    int                        cursorPos,
                                    std::function<bool(QChar)> exitCondition)
{
    if (direction == SearchDirection::left) {
        return moveLeft(str, cursorPos, exitCondition);
    } else {
        return moveRight(str, cursorPos, exitCondition);
    }
}

int TagProcessor::moveLeft(QStringView str, int cursorPos, std::function<bool(const QChar)> exitCondition)
{
    // Если мы у левой границы строки,
    // значит условие выхода не выполнилось - возвращаем -1
    if (cursorPos <= 0)
        return -1;

    const QChar leftChar = str[cursorPos - 1];

    if (exitCondition(leftChar)) {
        // Если для символа слева условие выполняется, возвращаем эту позицию
        return cursorPos;
    } else {
        // Иначе ищем левее
        return moveLeft(str, cursorPos - 1, exitCondition);
    }
}

int TagProcessor::moveRight(QStringView str, int cursorPos, std::function<bool(const QChar)> exitCondition)
{
    // Если мы у правой границы строки,
    // значит условие выхода не выполнилось - возвращаем -1
    if (cursorPos >= str.size() || cursorPos == -1)
        return -1;

    const QChar rightChar = str[cursorPos];

    if (exitCondition(rightChar)) {
        // Если для символа справа условие выполняется, возвращаем эту позицию
        return cursorPos;
    } else {
        // Иначе ищем правее
        return moveRight(str, cursorPos + 1, exitCondition);
    }
}

QString TagProcessor::replaceTags(QString str, const QString& leftBrReplacement, const QString& rightBrReplacement)
{
    if (!isPairedBrackets(str))
        return str;

    str.replace(leftBracket, leftBrReplacement);
    str.replace(rightBracket, rightBrReplacement);
    return str;
}

QStringList TagProcessor::extractTags(QString str)
{
    // На данном этапе считаем, что экранировать символы тегов нельзя
    // Функция работает только с корректными тегами
    // Если в строке непарные скобки или вложенность становится отрицательной
    // сразу возвращаем пустой список
    // То же самое для глубины вложенности скобок больше 1.
    // С такими вариантами тоже не работаем

    if (!isPairedBrackets(str))
        return {};

    if (getMaxDepthOfNestedBrackets(str) != 1)
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

/// Описание:
/// Функция в пустой строке или строке с пробелами вернет {}
/// Функция на границе строки обрамит крайнее слово
/// При встрече уже обрамленного тега - ничего не сделает
QString TagProcessor::addTag(QString str, int cursorPosition)
{
    // Проверка корректности курсора
    if (!isValidCursor(QStringView(str), cursorPosition))
        return str;

    // Нельзя просто так упрощать строку - индекс курсора же не меняется
    // str = str.simplified();

    // Если мы уже находимся внутри тега - ничего не делаем
    if (isInsideTag(QStringView(str), cursorPosition))
        return str;

    int leftWordBorder  = searchWordBorder(SearchDirection::left, str, cursorPosition);
    int rightWordBorder = searchWordBorder(SearchDirection::right, str, cursorPosition);

    //Сначала вставляем правую, потом левую из за смещения курсора
    str.insert(rightWordBorder, rightBracket);
    str.insert(leftWordBorder, leftBracket);

    return str;
}

QString TagProcessor::removeTag(QString str, int cursorPosition)
{
    // Проверка корректности курсора
    if (!isValidCursor(QStringView(str), cursorPosition))
        return str;

    // Нельзя просто так упрощать строку - индекс курсора же не меняется
    // str = str.simplified();

    // Если мы не находимся внутри тега - ничего не делаем
    if (!isInsideTag(QStringView(str), cursorPosition))
        return str;

    auto leftBracketPos  = getCursorPosition(SearchDirection::left, str, cursorPosition, isBracket);
    auto rightBracketPos = getCursorPosition(SearchDirection::right, str, cursorPosition, isBracket);

    //Сначала удаляем правую, потом левую из за смещения индексов
    str.remove(rightBracketPos, 1);
    str.remove(leftBracketPos - 1, 1);

    return str;
}

QString TagProcessor::expandTagRight(QString str, int cursorPosition)
{
    if (!isValidCursor(QStringView(str), cursorPosition))
        return str;

    if (!isInsideTag(QStringView(str), cursorPosition))
        return str;

    // Move to right bracket
    auto bracketPos = getCursorPosition(SearchDirection::right, str, cursorPosition, isBracket);
    // Move to word
    auto wordStartPos   = getCursorPosition(SearchDirection::right, str, bracketPos + 1, isLetterOrNumber);
    auto openBracketPos = getCursorPosition(SearchDirection::right, str, bracketPos + 1, isLeftBracket);

    // Protection from capturing next tag
    if (openBracketPos != -1) {
        if (wordStartPos > openBracketPos) {
            return str;
        }
    }

    auto wordEndPos = getCursorPosition(SearchDirection::right, str, wordStartPos, isLetterOrNumberInverse);

    if (wordStartPos != -1 && wordEndPos == -1) {
        wordEndPos = str.size();
    }

    if (bracketPos != -1 && wordEndPos != -1) {
        str.insert(wordEndPos, rightBracket);
        str.remove(bracketPos, 1);
        return str;
    }

    return str;
}

QString TagProcessor::decorateTags(QString str)
{
    return replaceTags(str, "<font color=\"#6d9a28\">", "</font>");
}
