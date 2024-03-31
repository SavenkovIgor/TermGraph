// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <optional>
#include <vector>

#include <QDateTime>

#include <CommonTools/HandyTypes.h>

#include "GroupSummary.h"
#include "TermData.h"

struct StaticGroupData : public GroupSummary
{
    using List = std::vector<StaticGroupData>;

    TermData::List terms;

    Opt<QDateTime> termsLastEdit() const;

    QMap<TermUuid, TermData> termsMap() const;

    Opt<TermData> term(const QString& termName) const;

    inline bool operator==(const StaticGroupData& rhs) const = default;

    // --- JSON ---
    static Opt<StaticGroupData> from(QJsonObject json);
    static Opt<StaticGroupData> from(const QByteArray& jsonBytes);

    QJsonObject toQJsonObject() const;

    explicit operator QJsonObject() const { return toQJsonObject(); }
    explicit operator QByteArray() const { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }
};
