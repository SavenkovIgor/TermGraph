// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <optional>
#include <vector>

#include <QDateTime>

#include <CommonTools/HandyTypes.h>
#include <TermDataInterface/TermData.h>
#include <TermDataInterface/GroupSummary.h>

struct GroupData: public GroupSummary
{
    TermData::List terms;

    inline bool operator==(const GroupData& rhs) const = default;

    // --- JSON ---
    static inline Opt<GroupData> from(QJsonObject json)
    {
        // Some import data fixes
        // Update of termsKey if need
        json = JsonTools::updateKey(json, JsonTools::oldTermsKey, JsonTools::termsKey);

        if (!GroupJsonValidator::fullChecks().check(json)) {
            return std::nullopt;
        }

        GroupData ret;

        ret.uuid     = GroupUuid::from(json[JsonTools::uuidKey].toString());
        ret.name     = json[JsonTools::nameKey].toString();
        ret.comment  = json[JsonTools::commentKey].toString();
        ret.size     = asInt(json[JsonTools::termsKey].toArray().size());
        ret.lastEdit = QDateTime::fromString(json[JsonTools::lastEditKey].toString(), Qt::ISODate);
        ret.terms    = TermData::List::from(json[JsonTools::termsKey].toArray());

        ret.nodesLastEdit = QDateTime::fromString(json[JsonTools::nodesLastEditKey].toString(), Qt::ISODate);

        if (ret.nodesLastEdit->isNull()) {
            QDateTime lastEdit;

            for (const auto& term : ret.terms) {
                if (term.lastEdit > lastEdit) {
                    lastEdit = term.lastEdit;
                }
            }

            ret.nodesLastEdit = lastEdit;
        }

        if (ret.nodesLastEdit->isNull()) {
            ret.nodesLastEdit = std::nullopt;
        }

        return ret;
    }

    static inline Opt<GroupData> from(const QByteArray& jsonBytes)
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

        ret.insert(JsonTools::termsKey, static_cast<QJsonArray>(terms));

        return ret;
    }

    explicit operator QByteArray() const { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }
};
