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

#include "source/Helpers/textprocessor.h"

TextProcessor::TextProcessor(QObject* parent)
    : QObject(parent)
{
    if (splitters.isEmpty()) {
        splitters << "‐";
        splitters << "-";
        splitters << "-";
        splitters << "—";
    }
}

bool TextProcessor::isTermWithDefinition(const QString& def) const
{
    for (const auto& splitter : splitters) {
        if (def.contains(splitter)) {
            return true;
        }
    }

    return false;
}

QString TextProcessor::getTerm(const QString& def) const
{
    if (int pos = splitterIndex(def); pos != -1) {
        return def.left(pos).simplified();
    }
    return "";
}

QString TextProcessor::getDefinition(const QString& def) const
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

int TextProcessor::splitterIndex(const QString& str) const
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
