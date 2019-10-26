/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
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

#include <QList>
#include <QString>

struct TColumn
{
    using List = QList<TColumn>;  // TODO: replace with vector

    const char* name;  // Name of column
    const char* type;  // Type description of column

    TColumn(const char* name)
        : name(name)
        , type("")
    {}

    constexpr TColumn(const char* name, const char* type)
        : name(name)
        , type(type)
    {}

    constexpr TColumn(const TColumn& col)
        : name(col.name)
        , type(col.type)
    {}

    operator QString() const {
        return QString(name);
    }

    bool operator ==(const TColumn& otherCol) const {
        QString nameSelf(name);
        QString nameOther(otherCol.name);

        QString typeSelf(type);
        QString typeOther(otherCol.type);
        return nameSelf == nameOther && typeSelf == typeOther;
    }
};
