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
    static inline QString toQString(const QJsonObject& obj)
    {
        QJsonDocument doc(obj);
        return QString(doc.toJson());
    }

    static inline std::string toStdString(const QJsonObject& obj) { return toQString(obj).toStdString(); }

    static inline std::string toStdString(const QString& rootKey, const QJsonArray& data)
    {
        QJsonObject obj;
        obj.insert(rootKey, data);
        return toStdString(obj);
    }

    static inline std::string toStdString(const QString& rootKey, const auto& uuidList)
    {
        QJsonArray arr;

        for (const auto& uuid : uuidList)
            arr.push_back(uuid.toString());

        return toStdString(rootKey, arr);
    }

    static inline std::string containerToStdString(const QString& rootKey, const auto& container)
    {
        QJsonArray arr;

        for (auto item : container)
            arr.push_back(item.toJson());

        return toStdString(rootKey, arr);
    }

    static inline QString prepareUuidParameter(const std::string& urlParameter)
    {
        auto uuidStr = QString::fromStdString(urlParameter);

        uuidStr.remove("%7B");
        uuidStr.remove("%7D");

        uuidStr.prepend('{');
        uuidStr.append('}');

        return uuidStr;
    }

    static inline Opt<QJsonObject> toJsonObject(const std::string& jsonStr)
    {
        auto bytes = QByteArray::fromStdString(jsonStr);
        auto doc   = QJsonDocument::fromJson(bytes);

        if (doc.isNull())
            return std::nullopt;

        return doc.object();
    }
};
