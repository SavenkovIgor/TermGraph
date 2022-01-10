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

#include <QJsonDocument>
#include <QJsonObject>

#include <CommonTools/GroupUuid.h>
#include <CommonTools/HandyTypes.h>
#include <CommonTools/SafeUuid.h>
#include <CommonTools/TermUuid.h>

class JsonTools
{
public:
    // QJsonObject
    static inline QString toQString(const QJsonObject& obj) { return QString(QJsonDocument(obj).toJson()); }

    // RootKey + jsonArray
    static inline QString toQString(const QString& rootKey, const QJsonArray& data)
    {
        QJsonObject obj;
        obj.insert(rootKey, data);
        return toQString(obj);
    }

    // Uuid list
    static inline QString toQString(const QString& rootKey, const auto& uuidList)
    {
        QJsonArray arr;

        for (const auto& uuid : uuidList)
            arr.push_back(uuid.toString());

        return toQString(rootKey, arr);
    }

    // Container array
    static inline QString containerToQString(const QString& rootKey, const auto& container)
    {
        QJsonArray arr;

        for (auto item : container)
            arr.push_back(item.toJson());

        return toQString(rootKey, arr);
    }

    static inline QString prepareUuidParameter(QString param)
    {
        param.remove("%7B");
        param.remove("%7D");

        param.prepend('{');
        param.append('}');

        return param;
    }

    static inline Opt<QJsonObject> toJsonObject(const std::string& jsonStr)
    {
        auto bytes = QByteArray::fromStdString(jsonStr);
        auto doc   = QJsonDocument::fromJson(bytes);

        if (doc.isNull())
            return std::nullopt;

        return doc.object();
    }

    constexpr auto groupsKey     = "groups";
    constexpr auto groupUuidsKey = "groupUuids";

    constexpr auto termsKey     = "terms";
    constexpr auto termUuidsKey = "termUuids"
};
