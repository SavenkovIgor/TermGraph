// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/CommonTools/TermUuid.h"

#include <QByteArray>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

import CommonTools.JsonKeys;
import CommonTools.JsonTools;

std::optional<TermUuid> TermUuid::from(QString text, UuidMode mode)
{
    if (mode == UuidMode::Url)
        text = jsonTools::prepareUuidParameter(text);

    if (auto safe = SafeUuid::from(text))
        return TermUuid(text);

    return std::nullopt;
}

std::optional<TermUuid> TermUuid::from(const QUuid& uuid)
{
    if (auto safe = SafeUuid::from(uuid))
        return TermUuid(uuid.toString());

    return std::nullopt;
}

TermUuid TermUuid::generate() { return TermUuid(QUuid::createUuid().toString()); }

std::optional<TermUuid::List> TermUuid::List::from(const QJsonObject& obj)
{
    List ret;

    if (!obj[jsonKeys::termUuidsKey].isArray())
        return std::nullopt;

    for (const auto& obj : obj[jsonKeys::termUuidsKey].toArray()) {
        if (auto uuid = TermUuid::from(obj.toString())) {
            ret.push_back(*uuid);
        } else {
            qWarning("Wrong uuid in received data");
        }
    }

    return ret;
}

std::optional<TermUuid::List> TermUuid::List::from(const QByteArray& jsonBytes)
{
    auto doc = QJsonDocument::fromJson(jsonBytes);

    if (doc.isNull())
        return std::nullopt;

    return from(doc.object());
}

TermUuid::List::operator QJsonObject() const
{
    QJsonArray arr;

    for (auto item : *this)
        arr.push_back(item.toString(StringFormat::WithoutBraces));

    QJsonObject obj;
    obj.insert(jsonKeys::termUuidsKey, arr);
    return obj;
}

TermUuid::List::operator QByteArray() const { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }

TermUuid::TermUuid(const QString& text)
    : SafeUuid(text)
{}
