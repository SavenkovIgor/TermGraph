#include "tagprocessor.h"

const QChar TagProcessor::leftBracket = '{';
const QChar TagProcessor::rightBracket = '}';

TagProcessor::TagProcessor(QObject *parent) : QObject(parent)
{ }

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

QStringList TagProcessor::extractTags(QString str, QString &errorString)
{
    // TODO: Разбить функцию на несколько.
    // Она не должна проверять ошибки.
    // Она должна только найти теги, какие сможет
    errorString = "";

    QStringList ret;
    QString tmpStr;
    bool skipNext = false;
    bool insideTag = false;

    for (int i = 0; i < str.size(); i++) {
        QChar c = str[i];
        if (insideTag) {
            if (c == '\\') {
                // Поймали экранирующий символ.
                // Следующий символ пропускаем без вопросов
                skipNext = true;
                continue;
            }
            if (skipNext) {
                tmpStr += c;
                skipNext = false;  // Отменяем пропуск
                continue;
            }
            if (c == '{') {
                errorString = "Неэкранированный символ { внутри тега";
                return QStringList();
            }
            if (c == '}') {  // Тег кончился - заносим в список
                insideTag = false;
                ret << tmpStr;
                tmpStr.clear();
                continue;
            }
            if (i == str.size() - 1) {
                errorString = "Незакрытый тег в конце строки";
                return QStringList();
            }
            tmpStr += c;
        }
        if (c == '{')  // Заходим в тег
            insideTag = true;
    }
    return ret;
}

/// Описание:
/// Функция в пустой строке или строке с пробелами вернет {}
/// Функция на границе строки обрамит крайнее слово
/// При встрече уже обрамленного тега - ничего не сделает
QString TagProcessor::addTagInPosition(int cursorPosition, QString str)
{
    // str = str.simplified();
    // Нельзя просто так упрощать строку - индекс курсора же не меняется

    if (str.simplified() == "")
        // Пустые строчки обслуживаем не считая.
        // Так же мы гарантируем, что в строчке
        // будет хотя бы один непробельный символ.
        return "{}";

    int startPos = -1, endPos = -1, lCursor = -1, rCursor = -1;
    //Позиции курсора, куда пихать потом скобки
    //И временные смещения курсора
    QChar lCurLChar, lCurRChar;
    //символы слева и справа от курсора у левого курсора
    //По умолчанию - null символы
    QChar rCurLChar, rCurRChar;
    //символы слева и справа от курсора у левого курсора

    //проблема столбов и заборов pos - позиция курсора а не символа

    for (int i = 0; i < 400; i++) {
        lCursor = qBound(0, cursorPosition - i, str.size());
        //Движемся влево  str.size() - НЕ ОШИБКА!
        rCursor = qBound(0, cursorPosition + i, str.size());
        //Движемся вправо str.size() - НЕ ОШИБКА!

        if (lCursor == 0) {
            lCurLChar = QChar();  // Слева пусто
            lCurRChar = str[ lCursor ];
        } else if (lCursor == str.size()) {
            lCurLChar = str[ lCursor - 1 ];
            lCurRChar = QChar();
        } else {  // Мы где то в середине
            lCurLChar = str[ lCursor - 1 ];
            lCurRChar = str[ lCursor ];
        }

        if (rCursor == 0) {
            rCurLChar = QChar();
            rCurRChar = str[ rCursor ];
        } else if (rCursor == str.size()) {
            rCurLChar = str[ rCursor - 1 ];
            rCurRChar = QChar();
        } else {
            rCurLChar = str[ rCursor - 1 ];
            rCurRChar = str[ rCursor ];
        }

        if (startPos != -1 && endPos != -1)
            // Нашли обе позиции вставки, выходим
            break;

        if (startPos == -1) {  // Если еще не нашли, то ищем
            // Если один из символов пустой - то мы уже нашли.
            if (lCurLChar.isNull())
                startPos = lCursor;
            // поскольку пустые строки у нас невозможны,
            // значит мы по любому в середине строки
            else if (lCurLChar.isSpace())
                startPos = lCursor;
        }

        if (endPos == -1) {  // Если еще не нашли, то ищем
            // Если один из символов пустой - то мы уже нашли.
            if (rCurRChar.isNull())
                endPos = rCursor;
            // поскольку пустые строки у нас невозможны,
            // значит мы по любому в середине строки
            else if (rCurRChar.isSpace())
                endPos = rCursor;
        }
    }

    //Сначала конец вставляем, потом начало из за смещения индексов
    //Если у нас пустой тег - нафиг он нужен
    //Случай с пустой строкой - уже вверху рассматривали
    if (startPos != endPos) {
        if (str[startPos] != '{' && str[endPos - 1] != '}') {
            // Защита от вписывания дополнительных тегов
            // TODO: написать функцию проверки уже в теге?
            // Чтобы избегать случаев вида: asdf {asdf {asdf}} asdf
            str.insert(endPos, '}');
            str.insert(startPos, '{');
        }
    }

    return str;
}
