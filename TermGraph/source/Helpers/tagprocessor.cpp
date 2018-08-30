#include "tagprocessor.h"

const QChar TagProcessor::leftBracket = '{';
const QChar TagProcessor::rightBracket = '}';
const QString TagProcessor::emptyBrackets = "{}";

TagProcessor::TagProcessor(QObject *parent) : QObject(parent)
{ }

bool TagProcessor::isBracket(QChar ch)
{
    return ch == leftBracket || ch == rightBracket;
}

int TagProcessor::searchWordBorder(const SearchDirection searchDirection, const QString& text, int cursorPosition)
{
    if (searchDirection == SearchDirection::left) {
        if (cursorPosition == 0) {
            // Если мы у левой границы строки - возвращаем эту границу
            return cursorPosition;
        }

        QChar leftChar = text[cursorPosition - 1];

        if (leftChar.isLetterOrNumber()) {  // Если это символ или цифра - ищем левее
            return searchWordBorder(searchDirection, text, cursorPosition - 1);
        } else {
            return cursorPosition;
        }
    } else {
        if (cursorPosition == text.size()) {
            // Если мы у правой границы строки - возвращаем эту границу
            return cursorPosition;
        }

        QChar rightChar = text[cursorPosition];

        if (rightChar.isLetterOrNumber()) {
            return searchWordBorder(searchDirection, text, cursorPosition + 1);
        } else {
            return cursorPosition;
        }
    }
}

QChar TagProcessor::getNearesBracket(const SearchDirection searchDirection, const QString &text, int cursorPosition)
{
    if (searchDirection == SearchDirection::left) {
        if (cursorPosition == 0) {
            // Если мы у левой границы строки - возвращаем пустой символ
            return QChar();
        }

        QChar leftChar = text[cursorPosition - 1];

        if (isBracket(leftChar)) {
            // Если это одна из скобок, возвращаем ее
            return leftChar;
        } else {
            // Иначе ищем левее
            return getNearesBracket(searchDirection, text, cursorPosition - 1);
        }
    } else {
        if (cursorPosition == text.size()) {
            // Если мы у правой границы строки - возвращаем пустой символ
            return QChar();
        }

        QChar rightChar = text[cursorPosition];

        if (isBracket(rightChar)) {
            // Если это одна из скобок, возвращаем ее
            return rightChar;
        } else {
            // Иначе ищем правее
            return getNearesBracket(searchDirection, text, cursorPosition + 1);
        }
    }
}

bool TagProcessor::isInsideTag(const QString &text, int cursorPosition)
{
    QChar firstLeftBracket = getNearesBracket(SearchDirection::left, text, cursorPosition);
    QChar firstRightBracket = getNearesBracket(SearchDirection::right, text, cursorPosition);

    if (firstLeftBracket == leftBracket && firstRightBracket == rightBracket) {
        return true;
    } else {
        return false;
    }
}

bool TagProcessor::isPairedBrackets(QString text)
{
    int depth = 0;
    for (QChar sym : text) {
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

int TagProcessor::getMaxDepthOfNestedBrackets(QString text)
{
    if (!isPairedBrackets(text))
        return -1;

    int depth = 0;
    int maxDepth = 0;
    for (QChar sym : text) {
        if (sym == leftBracket) {
            depth++;
        } else if (sym == rightBracket) {
            depth--;
        }
        maxDepth = qMax(maxDepth, depth);
    }
    return maxDepth;
}

int TagProcessor::getLevDistance(const QString &src, const QString &dst)
{
    const int m = src.size();
    const int n = dst.size();
    if (m == 0) {
        return n;
    }
    if (n == 0) {
        return m;
    }

    std::vector< std::vector<int> > matrix(m + 1);

    for (int i = 0; i <= m; ++i) {
        matrix[i].resize(n + 1);
        matrix[i][0] = i;
    }
    for (int i = 0; i <= n; ++i) {
        matrix[0][i] = i;
    }

    int above_cell, left_cell, diagonal_cell, cost;

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            cost = src[i - 1] == dst[j - 1] ? 0 : 1;
            above_cell = matrix[i - 1][j];
            left_cell = matrix[i][j - 1];
            diagonal_cell = matrix[i - 1][j - 1];
            matrix[i][j] = std::min(std::min(above_cell + 1, left_cell + 1), diagonal_cell + cost);
        }
    }

    return matrix[m][n];
}

QStringList TagProcessor::extractTags(QString str)
{
    // На данном этапе считаем, что экранировать символы тегов нельзя
    // Функция работает только с корректными тегами
    // Если в строке непарные скобки или вложенность становится отрицательной
    // сразу возвращаем пустой список
    // То же самое для глубины вложенности скобок больше 1.
    // С такими вариантами тоже не работаем

    if (!isPairedBrackets(str)) {
        return QStringList();
    }

    if (getMaxDepthOfNestedBrackets(str) != 1) {
        return QStringList();
    }

    QStringList tags;
    QString wordBuffer;
    bool insideTag = false;

    for (QChar symbol : str) {
        if (insideTag) {
            if (symbol == rightBracket) {  // Тег кончился - заносим в список
                insideTag = false;
                tags << wordBuffer;
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
    if (cursorPosition < 0 || cursorPosition > str.size()) {
        return str;
    }

    // Нельзя просто так упрощать строку - индекс курсора же не меняется
    // str = str.simplified();

    // Если мы уже находимся внутри тега - ничего не делаем
    if (isInsideTag(str, cursorPosition)) {
        return str;
    }

    int leftWordBorder = searchWordBorder(SearchDirection::left, str, cursorPosition);
    int rightWordBorder = searchWordBorder(SearchDirection::right, str, cursorPosition);

    //Сначала вставляем правую, потом левую из за смещения курсора
    str.insert(rightWordBorder, rightBracket);
    str.insert(leftWordBorder, leftBracket);

    return str;
}
