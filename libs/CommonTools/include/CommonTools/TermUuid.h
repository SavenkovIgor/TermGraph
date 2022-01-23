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

class TermUuid final : public SafeUuid
{
public:
    inline static Opt<TermUuid> create(QString text, UuidMode mode = UuidMode::Default)
    {
        if (mode == UuidMode::Url)
            text = JsonTools::prepareUuidParameter(text);

        if (auto safe = SafeUuid::create(text))
            return TermUuid(text);

        return std::nullopt;
    }

    inline static Opt<TermUuid> create(const QUuid& uuid)
    {
        if (auto safe = SafeUuid::create(uuid))
            return TermUuid(uuid.toString());

        return std::nullopt;
    }

    inline static TermUuid generate() { return TermUuid(QUuid::createUuid().toString()); }

    class List : public std::vector<TermUuid>
    {
    public:
        static inline Opt<List> create(const QJsonObject& obj)
        {
            Q_UNIMPLEMENTED();
            return std::nullopt;
        }

        static inline Opt<List> create(const QByteArray& jsonBytes)
        {
            Q_UNIMPLEMENTED();
            return std::nullopt;
        }

        explicit operator QJsonObject()
        {
            QJsonArray arr;

            for (auto item : *this)
                arr.push_back(item.toString(StringFormat::WithoutBraces));

            QJsonObject obj;
            obj.insert(JsonTools::termUuidsKey, arr);
            return obj;
        }

        explicit operator QByteArray() { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }
    };

private:
    explicit inline TermUuid(const QString& text)
        : SafeUuid(text)
    {}
};
