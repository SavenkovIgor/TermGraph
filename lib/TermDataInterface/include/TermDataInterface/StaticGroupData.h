// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <optional>
#include <vector>

#include <QDateTime>

#include <CommonTools/HandyTypes.h>
#include <TermDataInterface/TermData.h>
#include <TermDataInterface/GroupSummary.h>

struct StaticGroupData : public GroupSummary
{
    using List = std::vector<StaticGroupData>;

    TermData::List terms;

    inline Opt<QDateTime> termsLastEdit() const {
        if (terms.empty()) {
            return std::nullopt;
        }

        static TermData::List::size_type lastCountedSize = 0;
        static QDateTime lastCountedLastEdit;

        if (terms.size() == lastCountedSize) {
            return lastCountedLastEdit;
        }

        QDateTime ret = terms.front().lastEdit;

        for (const auto& term : terms) {
            if (term.lastEdit > ret) {
                ret = term.lastEdit;
            }
        }

        lastCountedSize = terms.size();
        lastCountedLastEdit = ret;

        return ret;
    }

    inline QMap<TermUuid, TermData> termsMap() const {
        QMap<TermUuid, TermData> ret;

        for (const auto& term : terms) {
            if (term.uuid) {
                ret.insert(term.uuid.value(), term);
            }
        }

        return ret;
    }

    inline Opt<TermData> term(const QString& termName) const {
        for (const auto& termData : terms) {
            if (termData.term == termName) {
                return termData;
            }
        }

        return std::nullopt;
    }

    inline bool operator==(const StaticGroupData& rhs) const = default;

    // --- JSON ---
    static inline Opt<StaticGroupData> from(QJsonObject json)
    {
        // Some import data fixes
        // Update of termsKey if need
        json = JsonTools::updateKey(json, JsonTools::oldTermsKey, JsonTools::termsKey);

        if (!GroupJsonValidator::staticDataChecks().check(json)) {
            return std::nullopt;
        }

        StaticGroupData ret;

        ret.uuid     = GroupUuid::from(json[JsonTools::uuidKey].toString());
        ret.name     = json[JsonTools::nameKey].toString();
        ret.comment  = json[JsonTools::commentKey].toString("");
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

    static inline Opt<StaticGroupData> from(const QByteArray& jsonBytes)
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
        ret = JsonTools::addIfNotEmpty(ret, JsonTools::commentKey, comment);
        ret.insert(JsonTools::lastEditKey, lastEdit.toString(Qt::ISODate));

        if (nodesLastEdit)
            ret.insert(JsonTools::nodesLastEditKey, nodesLastEdit->toString(Qt::ISODate));

        ret.insert(JsonTools::termsKey, static_cast<QJsonArray>(terms));

        return ret;
    }

    explicit operator QByteArray() const { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }
};
