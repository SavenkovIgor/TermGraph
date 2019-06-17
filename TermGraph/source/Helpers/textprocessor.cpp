#include "textprocessor.h"

TextProcessor::TextProcessor(QObject *parent) :
    QObject(parent)
{
    if (splitters.isEmpty()) {
        splitters << "‐";
        splitters << "-";
        splitters << "-";
        splitters << "—";
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
        return def.mid(pos + 1).simplified();
    }
    return "";
}

QString TextProcessor::insertNewLineNearMiddle(const QString& str)
{
    auto tryReplaceSpaceWithNewLine = [](QCharRef ref) {
        if (ref == ' ') {
            ref = '\n';
            return true;
        }
        return false;
    };

    QString ret = str;
    int middleIndex = ret.size() / 2;

    for (int i = 0; i < middleIndex; i++) {
        int leftIndex = qBound(0, middleIndex - i, middleIndex);
        int rightIndex = qBound(middleIndex, middleIndex + i, ret.size() - 1);

        if (tryReplaceSpaceWithNewLine(ret[leftIndex]))
            break;

        if (tryReplaceSpaceWithNewLine(ret[rightIndex]))
            break;
    }

    return ret;
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
