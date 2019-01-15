#include "tagprocessor.h"

TagProcessor::TagProcessor(QObject *parent) : QObject(parent)
{ }

bool TagProcessor::isBracket(const QChar& ch)
{
    return ch == leftBracket || ch == rightBracket;
}

bool TagProcessor::isLetterOrNumber(const QChar &ch)
{
    return ch.isLetterOrNumber();
}

bool TagProcessor::isSpaceCharacter(const QChar &ch)
{
    return ch.isSpace();
}

int TagProcessor::searchWordBorder(const SearchDirection direction, const QString& text, int cursorPos)
{
    if (direction == SearchDirection::left) {
        if (cursorPos == 0) {
            // Если мы у левой границы строки - возвращаем эту границу
            return cursorPos;
        }

        QChar leftChar = text[cursorPos - 1];

        if (leftChar.isLetterOrNumber()) {  // Если это символ или цифра - ищем левее
            return searchWordBorder(direction, text, cursorPos - 1);
        } else {
            return cursorPos;
        }
    } else {
        if (cursorPos == text.size()) {
            // Если мы у правой границы строки - возвращаем эту границу
            return cursorPos;
        }

        QChar rightChar = text[cursorPos];

        if (rightChar.isLetterOrNumber()) {
            return searchWordBorder(direction, text, cursorPos + 1);
        } else {
            return cursorPos;
        }
    }
}

QChar TagProcessor::getNearesBracket(const SearchDirection direction, const QString &text, int cursorPos)
{
    auto pos = getCursorPosition(direction, text, cursorPos, isBracket);

    if (pos == -1) {
        return QChar();
    }

    switch (direction) {
    case SearchDirection::left: return text[pos-1];
    case SearchDirection::right: return text[pos];
    }

    return QChar();
}

bool TagProcessor::isInsideTag(const QString &text, int cursorPos)
{
    QChar firstLeftBracket = getNearesBracket(SearchDirection::left, text, cursorPos);
    QChar firstRightBracket = getNearesBracket(SearchDirection::right, text, cursorPos);

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

bool TagProcessor::isTagCorrespondToTermName(QString termName, QString tag)  // TODO: Maybe refactor
{
    // To lower capital
    termName = termName.toLower();
    tag = tag.toLower();

    // Exact match
    if (termName.size() == tag.size() && termName == tag) {
        return true;
    }

    int acceptableDistance = 4*(1 + termName.count(' '));  // Пропорционально количеству слов
    acceptableDistance = 4;
    // TODO: Сделать защиту от формирования двухсторонних связей
    // TODO: Найти способ вызывать функцию в mainScene addEdge
    // TODO: Переделать так чтобы это было предложением а не обязательным действием
    if (TagProcessor::getLevDistance(termName, tag) <= acceptableDistance) {
        if (termName.left(3) == tag.left(3)) {
            return true;
        }
    }

    return false;
}

int TagProcessor::getCursorPosition(
        const SearchDirection& direction,
        const QString& text,
        int cursorPos,
        std::function<bool(QChar)> exitCondition)
{
    if (cursorPos == -1) {
        return -1;
    }

    if (direction == SearchDirection::left) {
        if (cursorPos <= 0) {
            // Если мы у левой границы строки - возвращаем -1
            return -1;
        }

        QChar leftChar = text[cursorPos - 1];

        if (exitCondition(leftChar)) {
            // Если для символа слева условие выполняется, возвращаем эту позицию
            return cursorPos;
        } else {
            // Иначе ищем левее
            return getCursorPosition(direction, text, cursorPos - 1, exitCondition);
        }
    } else {
        if (cursorPos >= text.size()) {
            // Если мы у правой границы строки - возвращаем -1
            return -1;
        }

        QChar rightChar = text[cursorPos];

        if (exitCondition(rightChar)) {
            // Если для символа справа условие выполняется, возвращаем эту позицию
            return cursorPos;
        } else {
            // Иначе ищем правее
            return getCursorPosition(direction, text, cursorPos + 1, exitCondition);
        }
    }
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

QString TagProcessor::extendRight(int cursorPosition, QString str)
{
    if (cursorPosition < 0 || cursorPosition > str.size()) {
        return str;
    }

    if (!isInsideTag(str, cursorPosition)) {
        return str;
    }

    // Move to right bracket
    auto bracketPos = getCursorPosition(SearchDirection::right, str, cursorPosition, isBracket);
    // Move to word
    auto wordStartPos = getCursorPosition(SearchDirection::right, str, bracketPos + 1, isLetterOrNumber);
    auto wordEndPos = getCursorPosition(SearchDirection::right, str, wordStartPos, isSpaceCharacter);

    if (bracketPos != -1 && wordEndPos != -1) {
        str.insert(wordEndPos, rightBracket);
        str.remove(bracketPos,1);
        return str;
    }

    return str;
}
