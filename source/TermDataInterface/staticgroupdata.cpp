// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/TermDataInterface/StaticGroupData.h"

#include "source/TermDataInterface/GroupValidator.h"

std::optional<QDateTime> StaticGroupData::termsLastEdit() const
{
    if (terms.empty()) {
        return std::nullopt;
    }

    static TermData::List::size_type lastCountedSize = 0;
    static QDateTime                 lastCountedLastEdit;

    if (terms.size() == lastCountedSize) {
        return lastCountedLastEdit;
    }

    QDateTime ret = terms.front().lastEdit;

    for (const auto& term : terms) {
        if (term.lastEdit > ret) {
            ret = term.lastEdit;
        }
    }

    lastCountedSize     = terms.size();
    lastCountedLastEdit = ret;

    return ret;
}

QMap<TermUuid, TermData> StaticGroupData::termsMap() const
{
    QMap<TermUuid, TermData> ret;

    for (const auto& term : terms) {
        if (term.uuid) {
            ret.insert(term.uuid.value(), term);
        }
    }

    return ret;
}

std::optional<TermData> StaticGroupData::term(const QString& termName) const
{
    for (const auto& termData : terms) {
        if (termData.term == termName) {
            return termData;
        }
    }

    return std::nullopt;
}

std::optional<StaticGroupData> StaticGroupData::from(QJsonObject json)
{
    // Some import data fixes
    // Update of termsKey if need
    json = JsonTools::updateKey(json, JsonTools::oldTermsKey, JsonTools::termsKey);

    if (!GroupJsonValidator::staticDataChecks().check(json)) {
        return std::nullopt;
    }

    StaticGroupData ret;

    if (json.contains(JsonTools::uuidKey)) {
        ret.uuid = GroupUuid::from(json[JsonTools::uuidKey].toString());
    } else {
        ret.uuid = std::nullopt;
    }
    ret.name    = json[JsonTools::nameKey].toString();
    ret.comment = json[JsonTools::commentKey].toString("");

    auto jsonTerms = json[JsonTools::termsKey].toArray();
    ret.terms      = TermData::List::from(jsonTerms, TermData::JsonCheckMode::Minimal);

    return ret;
}

std::optional<StaticGroupData> StaticGroupData::from(const QByteArray& jsonBytes)
{
    auto doc = QJsonDocument::fromJson(jsonBytes);

    if (doc.isNull())
        return std::nullopt;

    return from(doc.object());
}

QJsonObject StaticGroupData::toQJsonObject() const
{
    QJsonObject ret;

    if (uuid) {
        ret.insert(JsonTools::uuidKey, uuid->toString());
    }
    ret.insert(JsonTools::nameKey, name);
    ret = JsonTools::addIfNotEmpty(ret, JsonTools::commentKey, comment);

    QJsonArray jsonterms;

    for (const auto& term : terms) {
        jsonterms.append(term.toMinimalQJsonObject());
    }

    ret.insert(JsonTools::termsKey, jsonterms);

    return ret;
}
