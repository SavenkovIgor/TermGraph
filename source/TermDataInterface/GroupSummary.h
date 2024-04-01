// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <optional>
#include <vector>

#include <QByteArray>
#include <QJsonObject>
#include <QString>
#include <QUuid>

#include "source/CommonTools/GroupUuid.h"
#include "source/CommonTools/HandyTypes.h"
#include "source/CommonTools/JsonTools.h"

#include "GroupValidator.h"

// TODO: Make class and make fields private
struct GroupSummary
{
    Opt<GroupUuid> uuid;
    QString        name;
    QString        comment;

    int            size          = 0;
    QDateTime      lastEdit      = QDateTime();
    Opt<QDateTime> nodesLastEdit = std::nullopt;

    inline bool isNull() const { return uuid.has_value() && name.isEmpty(); }

    inline bool operator==(const GroupSummary& rhs) const = default;

    // --- JSON ---
    static Opt<GroupSummary> from(const QJsonObject& obj);
    static Opt<GroupSummary> from(const QByteArray& jsonBytes);

    QJsonObject toQJsonObject() const;

    explicit operator QJsonObject() const { return toQJsonObject(); }
    explicit operator QByteArray() const { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }

    class List : public std::vector<GroupSummary>
    {
    public:
        static Opt<List> from(const QJsonObject& obj);
        static Opt<List> from(const QByteArray& jsonBytes);

        QJsonObject toQJsonObject() const;

        explicit operator QJsonObject() const { return toQJsonObject(); }
        explicit operator QByteArray() const { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }
    };
};
