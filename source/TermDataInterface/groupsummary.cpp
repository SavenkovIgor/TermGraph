// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/TermDataInterface/GroupSummary.h"

import CommonTools.JsonTools;
import TermDataInterface.GroupValidator;

std::optional<GroupSummary> GroupSummary::from(const QJsonObject& obj)
{
    if (!GroupJsonValidator::defaultChecks().check(obj))
        return std::nullopt;

    GroupSummary ret;

    ret.uuid     = GroupUuid::from(obj[jsonTools::uuidKey].toString());
    ret.name     = obj[jsonTools::nameKey].toString();
    ret.comment  = obj[jsonTools::commentKey].toString();
    ret.size     = obj[jsonTools::sizeKey].toInt(0);
    ret.lastEdit = QDateTime::fromString(obj[jsonTools::lastEditKey].toString(), Qt::ISODate);

    auto nodeLastEdit = QDateTime::fromString(obj[jsonTools::nodesLastEditKey].toString(), Qt::ISODate);
    if (!nodeLastEdit.isNull())
        ret.nodesLastEdit = nodeLastEdit;
    else
        ret.nodesLastEdit = std::nullopt;

    assert(!ret.isNull());

    if (ret.isNull()) // Release safety
        return std::nullopt;

    return ret;
}

std::optional<GroupSummary> GroupSummary::from(const QByteArray& jsonBytes)
{
    auto doc = QJsonDocument::fromJson(jsonBytes);

    if (doc.isNull())
        return std::nullopt;

    return from(doc.object());
}

QJsonObject GroupSummary::toQJsonObject() const
{
    QJsonObject ret;

    ret.insert(jsonTools::uuidKey, (uuid ? uuid->toString() : ""));
    ret.insert(jsonTools::nameKey, name);
    ret.insert(jsonTools::commentKey, comment);
    ret.insert(jsonTools::sizeKey, size);
    ret.insert(jsonTools::lastEditKey, lastEdit.toString(Qt::ISODate));

    if (nodesLastEdit)
        ret.insert(jsonTools::nodesLastEditKey, nodesLastEdit->toString(Qt::ISODate));

    return ret;
}

std::optional<GroupSummary::List> GroupSummary::List::from(const QJsonObject& obj)
{
    List ret;

    if (!obj[jsonTools::groupsKey].isArray())
        return std::nullopt;

    for (const auto& groupJson : obj[jsonTools::groupsKey].toArray()) {
        if (auto groupInfo = GroupSummary::from(groupJson.toObject())) {
            ret.push_back(*groupInfo);
        } else {
            qWarning("Wrong groupInfo in received data");
        }
    }

    return ret;
}

std::optional<GroupSummary::List> GroupSummary::List::from(const QByteArray& jsonBytes)
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
    obj.insert(jsonTools::groupsKey, arr);
    return obj;
}
