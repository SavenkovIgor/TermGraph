#include "textprocessor.h"

TextProcessor::TextProcessor(QObject *parent) :
    QObject(parent)
{
    if (splitters.isEmpty()) {
        splitters << "‐";
        splitters << "-";
        splitters << "-";
    }
}

bool TextProcessor::isTermWithDefinition(const QString &def) const
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
    if (int pos = splitterIndex(def); pos != -1) {
        return def.left(pos).simplified();
    }
    return "";
}

QString TextProcessor::getDefinition(const QString &def) const
{
    if (int pos = splitterIndex(def); pos != -1) {
        return def.mid(pos+1).simplified();
    }
    return "";
}

int TextProcessor::splitterIndex(const QString &str) const
{
    int pos = -1;
    for (const auto& splitter : splitters) {
        pos = str.indexOf(splitter);
        if (pos != -1) {
            return pos;
        }
    }

    return pos;
}
