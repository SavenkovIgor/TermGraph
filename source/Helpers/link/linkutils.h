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

#pragma once

#include <functional>

#include <QChar>
#include <QString>
#include <QStringList>
#include <QStringView>

#include "source/Helpers/handytypes.h"
#include "source/Helpers/link/link.h"
#include "source/Helpers/text/textrange.h"

// Static class
class LinkUtils
{
public:
    // Tags
    static QString addTag(QString str, int cursor);
    static QString expandTagRight(QString str, int cursor);
    static QString removeTag(QString str, int cursor);

    static bool tagLengthSuitTerm(const QString& tag, const QString& termName);

    static int      getLevDistance(QStringView src, QStringView dst, int limit = 100000);
    static opt<int> getDistanceBetweenTagAndTerm(const QString& tag, const QString& termName, int maxLimit);

    // Words
    static int wordsCount(const QString& string);

    // Links
    static Link linkAt(QStringView str, int index);

    constexpr static int nullCursor = -1;
};
