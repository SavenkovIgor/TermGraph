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

#include <optional>
#include <vector>

#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <QUuid>

// TODO: Check tests!
struct GroupData
{
    using List = std::vector<GroupData>;

    QUuid   uuid;
    QString name;
    QString comment;

    inline bool isNull() const { return uuid.isNull() && name.isEmpty(); }

    inline bool operator==(const GroupData& rhs) const
    {
        return uuid == rhs.uuid && name == rhs.name && comment == rhs.comment;
    }

    // --- JSON ---
    // Returns valid object or nullopt
    static inline bool isValidJson(const QJsonObject& obj)
    {
        bool ret = true;

        ret &= obj[uuidKey].isString();
        ret &= !QUuid(obj[uuidKey].toString()).isNull();

        ret &= obj[nameKey].isString();
        ret &= !obj[nameKey].toString().isEmpty();

        ret &= obj[commentKey].isString();

        return ret;
    }

    static inline std::optional<GroupData> fromJson(const QJsonObject& obj)
    {
        if (!isValidJson(obj))
            return std::nullopt;

        GroupData ret;

        ret.uuid    = QUuid(obj[uuidKey].toString());
        ret.name    = obj[nameKey].toString();
        ret.comment = obj[commentKey].toString();

        assert(!ret.isNull());

        if (ret.isNull()) // Release safety
            return std::nullopt;

        return ret;
    }

    inline QJsonObject toJson() const
    {
        QJsonObject ret;

        ret.insert(uuidKey, uuid.toString());
        ret.insert(nameKey, name);
        ret.insert(commentKey, comment);

        return ret;
    }

private:
    constexpr static auto uuidKey    = "uuid";
    constexpr static auto nameKey    = "name";
    constexpr static auto commentKey = "comment";
};
