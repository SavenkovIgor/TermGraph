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

#include <CommonTools/JsonTools.h>
#include <CommonTools/SafeUuid.h>

class GroupUuid final : public SafeUuid
{
public:
    inline static Opt<GroupUuid> create(QString text, UuidMode mode = UuidMode::Default)
    {
        if (mode == UuidMode::Url)
            text = JsonTools::prepareUuidParameter(text);

        if (auto safe = SafeUuid::create(text))
            return GroupUuid(text);

        return std::nullopt;
    }

    inline static Opt<GroupUuid> create(const QUuid& uuid)
    {
        if (auto safe = SafeUuid::create(uuid))
            return GroupUuid(uuid.toString());

        return std::nullopt;
    }

    inline static GroupUuid generate() { return GroupUuid(QUuid::createUuid().toString()); }

    class List : public std::vector<GroupUuid>
    {
    public:
        static inline Opt<List> create(const QJsonObject& obj)
        {
            List ret;

            if (!obj[JsonTools::groupUuidsKey].isArray())
                return std::nullopt;

            for (const auto& obj : obj[JsonTools::groupUuidsKey].toArray()) {
                if (auto uuid = GroupUuid::create(obj.toString())) {
                    ret.push_back(*uuid);
                } else {
                    qWarning("Wrong uuid in received data");
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
                arr.push_back(item.toString(StringFormat::WithoutBraces));

            QJsonObject obj;
            obj.insert(JsonTools::groupUuidsKey, arr);
            return obj;
        }

        explicit operator QByteArray() { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }
    };

private:
    explicit inline GroupUuid(const QString& text)
        : SafeUuid(text)
    {}
};
