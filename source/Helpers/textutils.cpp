/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
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

#include "source/Helpers/textutils.h"

bool TextUtils::isTermWithDefinition(const QString &def)
{
    for (const auto &splitter : splitters()) {
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
    auto tryReplaceSpaceWithNewLine = [](QChar ref) {
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
    for (const auto &splitter : splitters()) {
        pos = str.indexOf(splitter);
        if (pos != -1) {
            return pos;
        }
    }

    return pos;
}

QStringList TextUtils::splitters()
{
    static QStringList ret;

    if (Q_UNLIKELY(ret.isEmpty())) {
        ret << "‐";
        ret << "-";
        ret << "-";
        ret << "—";
    }

    return ret;
}
