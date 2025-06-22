// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <vector>

#include <QDateTime>

#include "source/TermDataInterface/GroupSummary.h"
#include "source/TermDataInterface/TermData.h"

struct StaticGroupData : public GroupSummary
{
    using List = std::vector<StaticGroupData>;

    TermData::List terms;

    std::optional<QDateTime> termsLastEdit() const;

    QMap<TermUuid, TermData> termsMap() const;

    std::optional<TermData> term(const QString& termName) const;

    inline bool operator==(const StaticGroupData& rhs) const = default;

    // --- JSON ---
    static std::optional<StaticGroupData> from(QJsonObject json);
    static std::optional<StaticGroupData> from(const QByteArray& jsonBytes);

    QJsonObject toQJsonObject() const;

    explicit operator QJsonObject() const { return toQJsonObject(); }
    explicit operator QByteArray() const { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }
};
