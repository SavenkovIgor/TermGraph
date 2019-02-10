#include "textprocessor.h"

TextProcessor::TextProcessor(QObject *parent) :
    QObject(parent)
{
     splitters << "‐";
     splitters << "-";
}

bool TextProcessor::IsTermWithDefinition(const QString &def) const
{
    for (const auto& splitter : splitters) {
        if (def.contains(splitter)) {
            return true;
        }
    }

    return false;
}

QString TextProcessor::getTerm(const QString &def) const
{
    int pos = -1;
    for (const auto& splitter : splitters) {
        pos = def.indexOf(splitter);
        if (pos != -1) {
            break;
        }
    }

    if (pos == -1) {
        return "";
    }



}

QString TextProcessor::getDefinition(const QString &def) const
{

}
