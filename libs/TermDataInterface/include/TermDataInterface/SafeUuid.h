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

#include <QString>
#include <QUuid>

#include <CommonTools/HandyTypes.h>

class SafeUuid : public QUuid
{
public:
    using List = std::vector<SafeUuid>;

    inline static opt<SafeUuid> create(const QString& text)
    {
        auto uuid = QUuid(text);
        if (uuid.isNull())
            return std::nullopt;

        return SafeUuid(text);
    }

    inline static opt<SafeUuid> create(const QUuid& uuid)
    {
        if (uuid.isNull())
            return std::nullopt;

        return SafeUuid(uuid.toString());
    }

    inline static SafeUuid generate() { return {QUuid::createUuid().toString()}; }

    inline QUuid get() const { return QUuid(toString()); }

protected:
    inline SafeUuid(const QString& text)
        : QUuid(text)
    {
        assert(!isNull());
    }

    inline bool isNull() const { return false; }
};