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

#include <QByteArray>
#include <QJsonObject>
#include <QString>
#include <QUuid>

#include <CommonTools/HandyTypes.h>
#include <CommonTools/JsonTools.h>
#include <TermDataInterface/GroupValidator.h>

// TODO: Check tests!
// TODO: Make class and make fields private
struct GroupData
{
    QUuid   uuid;
    QString name;
    QString comment;

    inline bool isNull() const { return uuid.isNull() && name.isEmpty(); }

    inline bool operator==(const GroupData& rhs) const
    {
        return uuid == rhs.uuid && name == rhs.name && comment == rhs.comment;
    }

    // --- JSON ---
    static inline Opt<GroupData> create(const QJsonObject& obj)
    {
        if (!GroupJsonValidator::defaultChecks().check(obj))
            return std::nullopt;

        GroupData ret;

        ret.uuid    = QUuid(obj[GroupJsonValidator::uuidKey].toString());
        ret.name    = obj[GroupJsonValidator::nameKey].toString();
        ret.comment = obj[GroupJsonValidator::commentKey].toString();

        assert(!ret.isNull());

        if (ret.isNull()) // Release safety
            return std::nullopt;

        return ret;
    }

    static inline Opt<GroupData> create(const QByteArray& jsonBytes)
    {
        auto doc = QJsonDocument::fromJson(jsonBytes);

        if (doc.isNull())
            return std::nullopt;

        return create(doc.object());
    }

    operator QJsonObject()
    {
        QJsonObject ret;

        ret.insert(GroupJsonValidator::uuidKey, uuid.toString());
        ret.insert(GroupJsonValidator::nameKey, name);
        ret.insert(GroupJsonValidator::commentKey, comment);

        return ret;
    }

    explicit operator QByteArray() { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }

    class List : public std::vector<GroupData>
    {
    public:
        static inline Opt<List> create(const QJsonObject& obj)
        {
            List ret;

            if (!obj[JsonTools::groupsKey].isArray())
                return std::nullopt;

            for (const auto& groupJson : obj[JsonTools::groupsKey].toArray()) {
                if (auto groupData = GroupData::create(groupJson.toObject())) {
                    ret.push_back(*groupData);
                } else {
                    qWarning("Wrong groupData in received data");
                }
            }

            return ret;
        }

        static inline Opt<List> create(const QByteArray& jsonBytes)
        {
            auto doc = QJsonDocument::fromJson(jsonBytes);

            if (doc.isNull())
                return std::nullopt;

            return create(doc.object());
        }

        explicit operator QJsonObject()
        {
            QJsonArray arr;

            for (auto item : *this)
                arr.push_back(static_cast<QJsonObject>(item));

            QJsonObject obj;
            obj.insert(JsonTools::groupsKey, arr);
            return obj;
        }

        explicit operator QByteArray() { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }
    };
};
