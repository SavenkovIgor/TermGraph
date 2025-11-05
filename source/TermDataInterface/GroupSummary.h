// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <optional>
#include <vector>

#include <QByteArray>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QUuid>

#include "source/CommonTools/GroupUuid.h"

// TODO: Make class and make fields private
struct GroupSummary
{
    std::optional<GroupUuid> uuid;
    QString                  name;
    QString                  comment;

    int                      size          = 0;
    QDateTime                lastEdit      = QDateTime();
    std::optional<QDateTime> nodesLastEdit = std::nullopt;

    inline bool isNull() const { return uuid.has_value() && name.isEmpty(); }

    inline bool operator==(const GroupSummary& rhs) const = default;

    // --- JSON ---
    static std::optional<GroupSummary> from(const QJsonObject& obj);
    static std::optional<GroupSummary> from(const QByteArray& jsonBytes);

    QJsonObject toQJsonObject() const;

    explicit operator QJsonObject() const { return toQJsonObject(); }
    explicit operator QByteArray() const { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }

    class List : public std::vector<GroupSummary>
    {
    public:
        static std::optional<List> from(const QJsonObject& obj);
        static std::optional<List> from(const QByteArray& jsonBytes);

        QJsonObject toQJsonObject() const;

        explicit operator QJsonObject() const { return toQJsonObject(); }
        explicit operator QByteArray() const { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }
    };
};

// Inline implementations
import CommonTools.JsonKeys;
import TermDataInterface.GroupValidator;

inline std::optional<GroupSummary> GroupSummary::from(const QJsonObject& obj)
{
    if (!GroupJsonValidator::defaultChecks().check(obj))
        return std::nullopt;

    GroupSummary ret;

    ret.uuid     = GroupUuid::from(obj[jsonKeys::uuidKey].toString());
    ret.name     = obj[jsonKeys::nameKey].toString();
    ret.comment  = obj[jsonKeys::commentKey].toString();
    ret.size     = obj[jsonKeys::sizeKey].toInt(0);
    ret.lastEdit = QDateTime::fromString(obj[jsonKeys::lastEditKey].toString(), Qt::ISODate);

    auto nodeLastEdit = QDateTime::fromString(obj[jsonKeys::nodesLastEditKey].toString(), Qt::ISODate);
    if (!nodeLastEdit.isNull())
        ret.nodesLastEdit = nodeLastEdit;
    else
        ret.nodesLastEdit = std::nullopt;

    assert(!ret.isNull());

    if (ret.isNull()) // Release safety
        return std::nullopt;

    return ret;
}

inline std::optional<GroupSummary> GroupSummary::from(const QByteArray& jsonBytes)
{
    auto doc = QJsonDocument::fromJson(jsonBytes);

    if (doc.isNull())
        return std::nullopt;

    return from(doc.object());
}

inline QJsonObject GroupSummary::toQJsonObject() const
{
    QJsonObject ret;

    ret.insert(jsonKeys::uuidKey, (uuid ? uuid->toString() : ""));
    ret.insert(jsonKeys::nameKey, name);
    ret.insert(jsonKeys::commentKey, comment);
    ret.insert(jsonKeys::sizeKey, size);
    ret.insert(jsonKeys::lastEditKey, lastEdit.toString(Qt::ISODate));

    if (nodesLastEdit)
        ret.insert(jsonKeys::nodesLastEditKey, nodesLastEdit->toString(Qt::ISODate));

    return ret;
}

inline std::optional<GroupSummary::List> GroupSummary::List::from(const QJsonObject& obj)
{
    List ret;

    if (!obj[jsonKeys::groupsKey].isArray())
        return std::nullopt;

    for (const auto& groupJson : obj[jsonKeys::groupsKey].toArray()) {
        if (auto groupInfo = GroupSummary::from(groupJson.toObject())) {
            ret.push_back(*groupInfo);
        } else {
            qWarning("Wrong groupInfo in received data");
        }
    }

    return ret;
}

inline std::optional<GroupSummary::List> GroupSummary::List::from(const QByteArray& jsonBytes)
{
    auto doc = QJsonDocument::fromJson(jsonBytes);

    if (doc.isNull())
        return std::nullopt;

    return from(doc.object());
}

inline QJsonObject GroupSummary::List::toQJsonObject() const
{
    QJsonArray arr;

    for (const auto& item : *this)
        arr.push_back(static_cast<QJsonObject>(item));

    QJsonObject obj;
    obj.insert(jsonKeys::groupsKey, arr);
    return obj;
}
