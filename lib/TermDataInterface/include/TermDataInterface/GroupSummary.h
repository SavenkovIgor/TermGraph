// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <optional>
#include <vector>

#include <QByteArray>
#include <QJsonObject>
#include <QString>
#include <QUuid>

#include <CommonTools/GroupUuid.h>
#include <CommonTools/HandyTypes.h>
#include <CommonTools/JsonTools.h>
#include <TermDataInterface/GroupValidator.h>

// TODO: Check tests!
// TODO: Make class and make fields private
struct GroupSummary
{
    Opt<GroupUuid> uuid;
    QString name;
    QString comment;

    int            size          = 0;
    QDateTime      lastEdit      = QDateTime();
    Opt<QDateTime> nodesLastEdit = std::nullopt;

    inline bool isNull() const { return uuid.has_value() && name.isEmpty(); }

    inline bool operator==(const GroupSummary& rhs) const = default;

    // --- JSON ---
    static inline Opt<GroupSummary> from(const QJsonObject& obj)
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

    static inline Opt<GroupSummary> from(const QByteArray& jsonBytes)
    {
        auto doc = QJsonDocument::fromJson(jsonBytes);

        if (doc.isNull())
            return std::nullopt;

        return from(doc.object());
    }

    operator QJsonObject() const
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

    explicit operator QByteArray() const { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }

    class List : public std::vector<GroupSummary>
    {
    public:
        static inline Opt<List> from(const QJsonObject& obj)
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

        static inline Opt<List> from(const QByteArray& jsonBytes)
        {
            auto doc = QJsonDocument::fromJson(jsonBytes);

            if (doc.isNull())
                return std::nullopt;

            return from(doc.object());
        }

        explicit operator QJsonObject() const
        {
            QJsonArray arr;

            for (const auto& item : *this)
                arr.push_back(static_cast<QJsonObject>(item));

            QJsonObject obj;
            obj.insert(JsonTools::groupsKey, arr);
            return obj;
        }

        explicit operator QByteArray() const { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }
    };
};
