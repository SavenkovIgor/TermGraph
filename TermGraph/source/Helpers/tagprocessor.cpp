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

TagProcessor::TagProcessor(QObject* parent) : QObject(parent) {}

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

int TagProcessor::searchWordBorder(const SearchDirection direction, const QString& text, int cursorPos)
{
    auto pos = getCursorPosition(direction, text, cursorPos, isLetterOrNumberInverse);

    if (pos == -1) {
        switch (direction) {
        case SearchDirection::left:
            return 0;
        case SearchDirection::right:
            return text.size();
        }
    }

    return pos;
}

QChar TagProcessor::getNearesBracket(const SearchDirection direction, const QString& text, int cursorPos)
{
    auto pos = getCursorPosition(direction, text, cursorPos, isBracket);

    if (pos == -1)
        return {};

    switch (direction) {
    case SearchDirection::left:
        return text[pos - 1];
    case SearchDirection::right:
        return text[pos];
    }

    return {};
}

bool TagProcessor::isInsideTag(const QString& text, int cursorPos)
{
    QChar firstLeftBracket  = getNearesBracket(SearchDirection::left, text, cursorPos);
    QChar firstRightBracket = getNearesBracket(SearchDirection::right, text, cursorPos);

    return firstLeftBracket == leftBracket && firstRightBracket == rightBracket;
}

bool TagProcessor::isPairedBrackets(QStringView text)
{
    int depth = 0;
    for (auto sym : text) {
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

int TagProcessor::getMaxDepthOfNestedBrackets(QStringView text)
{
    if (!isPairedBrackets(text))
        return -1;

    int depth    = 0;
    int maxDepth = 0;
    for (auto sym : text) {
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

std::pair<bool, int> TagProcessor::isTagCorrespondToTermName(QString termName, QString tag)  // TODO: Maybe refactor
{
    // To lower capital
    termName = termName.toLower();
    tag      = tag.toLower();

    // Exact match
    if (termName.size() == tag.size() && termName == tag)
        return std::pair(true, 0);

    int acceptableDistance = 4 * (termName.count(' ') + 1);  // Пропорционально количеству слов
                                                             //    acceptableDistance = 4;
    // TODO: Сделать защиту от формирования двухсторонних связей
    // TODO: Найти способ вызывать функцию в mainScene addEdge
    // TODO: Переделать так чтобы это было предложением а не обязательным действием
    auto distance = TagProcessor::getLevDistance(termName, tag, acceptableDistance);
    if (distance <= acceptableDistance) {
        if (termName.left(3) == tag.left(3)) {
            return std::pair(true, distance);
        }
    }

    return std::pair(false, distance);
}

int TagProcessor::getCursorPosition(const SearchDirection&     direction,
                                    const QString&             text,
                                    int                        cursorPos,
                                    std::function<bool(QChar)> exitCondition)
{
    if (direction == SearchDirection::left) {
        return moveLeft(text, cursorPos, exitCondition);
    } else {
        return moveRight(text, cursorPos, exitCondition);
    }
}

int TagProcessor::moveLeft(const QString& text, int cursorPos, std::function<bool(const QChar)> exitCondition)
{
    if (cursorPos <= 0) {
        // Если мы у левой границы строки - возвращаем -1
        return -1;
    }

    const QChar leftChar = text[cursorPos - 1];

    if (exitCondition(leftChar)) {
        // Если для символа слева условие выполняется, возвращаем эту позицию
        return cursorPos;
    } else {
        // Иначе ищем левее
        return moveLeft(text, cursorPos - 1, exitCondition);
    }
}

int TagProcessor::moveRight(const QString& text, int cursorPos, std::function<bool(const QChar)> exitCondition)
{
    if (cursorPos >= text.size() || cursorPos == -1) {
        // Если мы у правой границы строки - возвращаем -1
        return -1;
    }

    const QChar rightChar = text[cursorPos];

    if (exitCondition(rightChar)) {
        // Если для символа справа условие выполняется, возвращаем эту позицию
        return cursorPos;
    } else {
        // Иначе ищем правее
        return moveRight(text, cursorPos + 1, exitCondition);
    }
}

QString TagProcessor::replaceTags(const QString& src,
                                  const QString& leftBrReplacement,
                                  const QString& rightBrReplacement)
{
    if (!isPairedBrackets(src))
        return src;

    QString ret = src;
    ret.replace(leftBracket, leftBrReplacement);
    ret.replace(rightBracket, rightBrReplacement);
    return ret;
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
QString TagProcessor::addTagInPosition(int cursorPosition, QString str)
{
    // Проверка корректности курсора
    if (!isValidCursor(cursorPosition, str))
        return str;

    // Нельзя просто так упрощать строку - индекс курсора же не меняется
    // str = str.simplified();

    // Если мы уже находимся внутри тега - ничего не делаем
    if (isInsideTag(str, cursorPosition))
        return str;

    int leftWordBorder  = searchWordBorder(SearchDirection::left, str, cursorPosition);
    int rightWordBorder = searchWordBorder(SearchDirection::right, str, cursorPosition);

    //Сначала вставляем правую, потом левую из за смещения курсора
    str.insert(rightWordBorder, rightBracket);
    str.insert(leftWordBorder, leftBracket);

    return str;
}

QString TagProcessor::removeTagInPosition(int cursorPosition, QString str)
{
    // Проверка корректности курсора
    if (!isValidCursor(cursorPosition, str))
        return str;

    // Нельзя просто так упрощать строку - индекс курсора же не меняется
    // str = str.simplified();

    // Если мы не находимся внутри тега - ничего не делаем
    if (!isInsideTag(str, cursorPosition))
        return str;

    auto leftBracketPos  = getCursorPosition(SearchDirection::left, str, cursorPosition, isBracket);
    auto rightBracketPos = getCursorPosition(SearchDirection::right, str, cursorPosition, isBracket);

    //Сначала удаляем правую, потом левую из за смещения индексов
    str.remove(rightBracketPos, 1);
    str.remove(leftBracketPos - 1, 1);

    return str;
}

QString TagProcessor::expandRight(int cursorPosition, QString str)
{
    if (!isValidCursor(cursorPosition, str))
        return str;

    if (!isInsideTag(str, cursorPosition))
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

QString TagProcessor::decorateTags(const QString& src)
{
    return replaceTags(src, "<font color=\"#6d9a28\">", "</font>");
}
