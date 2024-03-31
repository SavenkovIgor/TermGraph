// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/TermDataInterface/GroupSummary.h"

Opt<GroupSummary> GroupSummary::from(const QJsonObject& obj)
{
    if (!GroupJsonValidator::defaultChecks().check(obj))
        return std::nullopt;

    GroupSummary ret;

    ret.uuid     = GroupUuid::from(obj[JsonTools::uuidKey].toString());
    ret.name     = obj[JsonTools::nameKey].toString();
    ret.comment  = obj[JsonTools::commentKey].toString();
    ret.size     = obj[JsonTools::sizeKey].toInt(0);
    ret.lastEdit = QDateTime::fromString(obj[JsonTools::lastEditKey].toString(), Qt::ISODate);

    auto nodeLastEdit = QDateTime::fromString(obj[JsonTools::nodesLastEditKey].toString(), Qt::ISODate);
    if (!nodeLastEdit.isNull())
        ret.nodesLastEdit = nodeLastEdit;
    else
        ret.nodesLastEdit = std::nullopt;

    assert(!ret.isNull());

    if (ret.isNull()) // Release safety
        return std::nullopt;

    return ret;
}

Opt<GroupSummary> GroupSummary::from(const QByteArray& jsonBytes)
{
    auto doc = QJsonDocument::fromJson(jsonBytes);

    if (doc.isNull())
        return std::nullopt;

    return from(doc.object());
}

QJsonObject GroupSummary::toQJsonObject() const
{
    QJsonObject ret;

    ret.insert(JsonTools::uuidKey, (uuid ? uuid->toString() : ""));
    ret.insert(JsonTools::nameKey, name);
    ret.insert(JsonTools::commentKey, comment);
    ret.insert(JsonTools::sizeKey, size);
    ret.insert(JsonTools::lastEditKey, lastEdit.toString(Qt::ISODate));

    if (nodesLastEdit)
        ret.insert(JsonTools::nodesLastEditKey, nodesLastEdit->toString(Qt::ISODate));

    return ret;
}

Opt<GroupSummary::List> GroupSummary::List::from(const QJsonObject& obj)
{
    List ret;

    if (!obj[JsonTools::groupsKey].isArray())
        return std::nullopt;

    for (const auto& groupJson : obj[JsonTools::groupsKey].toArray()) {
        if (auto groupInfo = GroupSummary::from(groupJson.toObject())) {
            ret.push_back(*groupInfo);
        } else {
            qWarning("Wrong groupInfo in received data");
        }
    }

    return ret;
}

Opt<GroupSummary::List> GroupSummary::List::from(const QByteArray& jsonBytes)
{
    auto doc = QJsonDocument::fromJson(jsonBytes);

    if (doc.isNull())
        return std::nullopt;

    return from(doc.object());
}

QJsonObject GroupSummary::List::toQJsonObject() const
{
    QJsonArray arr;

    for (const auto& item : *this)
        arr.push_back(static_cast<QJsonObject>(item));

    QJsonObject obj;
    obj.insert(JsonTools::groupsKey, arr);
    return obj;
}
