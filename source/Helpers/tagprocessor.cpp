#include "tagprocessor.h"

TagProcessor::TagProcessor(QObject *parent) : QObject(parent)
{ }

QStringList TagProcessor::extractTags(QString str, QString &errorString)
{ //TODO: Разбить функцию на несколько. Она не должна проверять ошибки. Она должна только найти теги, какие сможет
    errorString = "";

    QStringList ret;
    QString tmpStr;
    bool skipNext = false;
    bool insideTag = false;

    for(int i = 0; i < str.size(); i++) {
        QChar c = str[i];
        if( insideTag ) {

            if( c == '\\' ) { //Поймали экранирующий символ. Следующий символ пропускаем без вопросов
                skipNext = true;
                continue;
            }
            if( skipNext ) {
                tmpStr += c;
                skipNext = false; //Отменяем пропуск
                continue;
            }
            if( c == '{' ) {
                errorString = "Неэкранированный символ { внутри тега";
                return QStringList();
            }
            if( c == '}' ) { //Тег кончился - заносим в список
                insideTag = false;
                ret << tmpStr;
                tmpStr.clear();
                continue;
            }
            if( i == str.size() - 1 ) {
                errorString = "Незакрытый тег в конце строки";
                return QStringList();
            }
            tmpStr += c;
        }
        if( c == '{') //Заходим в тег
            insideTag = true;
    }
    return ret;
}
