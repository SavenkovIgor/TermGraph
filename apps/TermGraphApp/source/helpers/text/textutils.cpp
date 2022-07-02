// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/helpers/text/textutils.h"

const QStringList TextUtils::mSplitters = {"‐", "-", "-", "—"};

bool TextUtils::isTermWithDefinition(const QString &def)
{
    for (const auto &splitter : mSplitters) {
        if (def.contains(splitter)) {
            return true;
        }
    }

    return false;
}

QString TextUtils::getTerm(const QString &def)
{
    if (int pos = splitterIndex(def); pos != -1) {
        return def.left(pos).simplified();
    }
    return "";
}

QString TextUtils::getDefinition(const QString &def)
{
    if (int pos = splitterIndex(def); pos != -1) {
        return def.mid(pos + 1).simplified();
    }
    return "";
}

QString TextUtils::insertNewLineNearMiddle(const QString &str)
{
    auto tryReplaceSpaceWithNewLine = [](QChar &ref) {
        if (ref == ' ') {
            ref = '\n';
            return true;
        }
        return false;
    };

    QString ret         = str;
    int     middleIndex = ret.size() / 2;

    for (int i = 0; i < middleIndex; i++) {
        int leftIndex  = qBound(0, middleIndex - i, middleIndex);
        int rightIndex = qBound(middleIndex, middleIndex + i, ret.size() - 1);

        if (tryReplaceSpaceWithNewLine(ret[leftIndex]))
            break;

        if (tryReplaceSpaceWithNewLine(ret[rightIndex]))
            break;
    }

    return ret;
}

int TextUtils::splitterIndex(const QString &str)
{
    int pos = -1;
    for (const auto &splitter : mSplitters) {
        pos = str.indexOf(splitter);
        if (pos != -1) {
            return pos;
        }
    }

    return pos;
}

int TextUtils::wordCount(const QString &str)
{
    const QString tmp = str.simplified();
    return !tmp.isEmpty() ? tmp.count(' ') + 1 : 0;
}
