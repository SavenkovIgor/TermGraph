// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/CommonTools/GroupUuid.h"

#include <QByteArray>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

import CommonTools.JsonKeys;
import CommonTools.JsonTools;

std::optional<GroupUuid> GroupUuid::from(QString text, UuidMode mode)
{
    if (mode == UuidMode::Url)
        text = jsonTools::prepareUuidParameter(text);

    if (auto safe = SafeUuid::from(text))
        return GroupUuid(text);

    return std::nullopt;
}

std::optional<GroupUuid> GroupUuid::from(const QUuid& uuid)
{
    if (auto safe = SafeUuid::from(uuid))
        return GroupUuid(uuid.toString());

    return std::nullopt;
}

GroupUuid GroupUuid::generate() { return GroupUuid(QUuid::createUuid().toString()); }

std::optional<GroupUuid::List> GroupUuid::List::from(const QJsonObject& obj)
{
    List ret;

    if (!obj[jsonKeys::groupUuidsKey].isArray())
        return std::nullopt;

    for (const auto& obj : obj[jsonKeys::groupUuidsKey].toArray()) {
        if (auto uuid = GroupUuid::from(obj.toString())) {
            ret.push_back(*uuid);
        } else {
            qWarning("Wrong uuid in received data");
        }
    }

    return ret;
}

std::optional<GroupUuid::List> GroupUuid::List::from(const QByteArray& jsonBytes)
{
    auto doc = QJsonDocument::fromJson(jsonBytes);

    if (doc.isNull())
        return std::nullopt;

    return from(doc.object());
}

GroupUuid::List::operator QJsonObject() const
{
    QJsonArray arr;

    for (auto item : *this)
        arr.push_back(item.toString(StringFormat::WithoutBraces));

    QJsonObject obj;
    obj.insert(jsonKeys::groupUuidsKey, arr);
    return obj;
}

GroupUuid::List::operator QByteArray() const { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }

GroupUuid::GroupUuid(const QString& text)
    : SafeUuid(text)
{}
