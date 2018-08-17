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
    // str = str.simplified();
    // Нельзя просто так упрощать строку - индекс курсора же не меняется

    if (str.simplified() == "") {
        // Пустые строчки обслуживаем не считая.
        // Так же мы гарантируем, что в строчке
        // будет хотя бы один непробельный символ.
        return emptyBrackets;
    }

    // Если мы уже находимся внутри тега - ничего не делаем
    if (isInsideTag(str, cursorPosition)) {
        return str;
    }

    /*if (cursorPosition > 0 && cursorPosition < str.size()) {
        // Если мы в середине строки
        QChar leftChar = str[cursorPosition-1];
        QChar rightChar = str[cursorPosition];
    }
    */

    int leftWordBorder = searchWordBorder(SearchDirection::left, str, cursorPosition);
    int rightWordBorder = searchWordBorder(SearchDirection::right, str, cursorPosition);

    //Сначала вставляем правую, потом левую из за смещения курсора
    str.insert(rightWordBorder, rightBracket);
    str.insert(leftWordBorder, leftBracket);

    return str;
}
