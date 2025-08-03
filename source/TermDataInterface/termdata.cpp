// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/TermDataInterface/TermData.h"

#include <set>

import CommonTools.JsonTools;
import Text;
import CommonTools.TermJsonValidator;

TermData TermData::createGhost(const QString& term, GroupUuid groupUuid)
{
    return TermData({
        .uuid          = TermUuid::generate(),
        .term          = term,
        .definition    = "",
        .description   = "",
        .examples      = "",
        .wikiUrl       = "",
        .wikiImage     = "",
        .knowledgeArea = "",
        .groupUuid     = groupUuid,
        .lastEdit      = QDateTime::currentDateTime(),
    });
}

std::optional<TermData> TermData::from(const QJsonObject& obj, JsonCheckMode mode)
{
    bool checkUuid     = true;
    bool checkLastEdit = true;

    if (mode == JsonCheckMode::AddTerm) {
        checkUuid     = false;
        checkLastEdit = false;
    }

    auto validator = TermJsonValidator(checkUuid, checkLastEdit);

    if (mode == JsonCheckMode::Minimal) {
        validator = TermJsonValidator::minimalStaticDataChecks();
    }

    if (!validator.check(obj))
        return std::nullopt;

    std::optional<GroupUuid> gUuid;

    if (mode == JsonCheckMode::Minimal) {
        gUuid = GroupUuid::generate();
    } else {
        gUuid = GroupUuid::from(obj[jsonTools::groupUuidKey].toString());
    }

    if (!gUuid)
        return std::nullopt;

    QDateTime lastEdit;

    if (obj.contains(jsonTools::lastEditKey)) {
        lastEdit = QDateTime::fromString(obj[jsonTools::lastEditKey].toString(), Qt::ISODate);
    }

    std::optional<TermUuid> uuid;

    if (obj.contains(jsonTools::uuidKey)) {
        uuid = TermUuid::from(obj[jsonTools::uuidKey].toString());
    }

    QString term;
    QString definition;

    if (mode == JsonCheckMode::Minimal) {
        auto termDef = obj[jsonTools::termDefKey].toString();

        auto res = TextTools::splitTermAndDefinition(termDef);

        term       = res.first;
        definition = res.second;
    } else {
        term       = obj[jsonTools::termKey].toString();
        definition = obj[jsonTools::definitionKey].toString();
    }

    TermData ret{
        .uuid          = uuid,
        .term          = term,
        .definition    = definition,
        .description   = obj[jsonTools::descriptionKey].toString(""),
        .examples      = obj[jsonTools::examplesKey].toString(""),
        .wikiUrl       = obj[jsonTools::wikiUrlKey].toString(""),
        .wikiImage     = obj[jsonTools::wikiImageKey].toString(""),
        .knowledgeArea = obj[jsonTools::knowledgeAreaKey].toString(""),
        .groupUuid     = *gUuid,
        .lastEdit      = lastEdit,
    };

    if (ret.isNull()) // Release safety
        return std::nullopt;

    return ret;
}

std::optional<TermData> TermData::from(const QByteArray& jsonBytes, JsonCheckMode mode)
{
    auto doc = QJsonDocument::fromJson(jsonBytes);

    if (doc.isNull())
        return std::nullopt;

    return from(doc.object(), mode);
}

QJsonObject TermData::toQJsonObject() const
{
    QJsonObject ret;

    ret.insert(jsonTools::uuidKey, (uuid ? uuid->toString() : ""));
    ret.insert(jsonTools::termKey, term);
    ret.insert(jsonTools::definitionKey, definition);

    ret = jsonTools::addIfNotEmpty(ret, jsonTools::descriptionKey, description);
    ret = jsonTools::addIfNotEmpty(ret, jsonTools::examplesKey, examples);
    ret = jsonTools::addIfNotEmpty(ret, jsonTools::wikiUrlKey, wikiUrl);
    ret = jsonTools::addIfNotEmpty(ret, jsonTools::wikiImageKey, wikiImage);
    ret = jsonTools::addIfNotEmpty(ret, jsonTools::knowledgeAreaKey, knowledgeArea);

    ret.insert(jsonTools::groupUuidKey, groupUuid.toString());
    ret.insert(jsonTools::lastEditKey, lastEdit.toString(Qt::ISODate));

    return ret;
}

QJsonObject TermData::toMinimalQJsonObject() const
{
    QJsonObject ret;

    auto termDef = TextTools::joinTermDef(term, definition);

    ret.insert(jsonTools::termDefKey, termDef);

    ret = jsonTools::addIfNotEmpty(ret, jsonTools::descriptionKey, description);
    ret = jsonTools::addIfNotEmpty(ret, jsonTools::examplesKey, examples);
    ret = jsonTools::addIfNotEmpty(ret, jsonTools::wikiUrlKey, wikiUrl);
    ret = jsonTools::addIfNotEmpty(ret, jsonTools::wikiImageKey, wikiImage);
    ret = jsonTools::addIfNotEmpty(ret, jsonTools::knowledgeAreaKey, knowledgeArea);

    return ret;
}

QByteArray TermData::toQByteArray() const { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }

std::optional<TermData::List> TermData::List::from(const QJsonObject& obj)
{
    TermData::List ret;

    if (!obj[jsonTools::termsKey].isArray())
        return std::nullopt;

    return from(obj[jsonTools::termsKey].toArray());
}

TermData::List TermData::List::from(const QJsonArray& json, TermData::JsonCheckMode mode)
{
    TermData::List ret;

    for (const auto& termJson : json) {
        if (auto termData = TermData::from(termJson.toObject(), mode)) {
            ret.push_back(*termData);
        } else {
            qWarning() << "Wrong termData json array: " << termJson;
        }
    }

    // Check for duplicates and print warning
    std::set<QString> terms;
    for (const auto& term : ret) {
        if (terms.find(term.term) != terms.end()) {
            qWarning() << "Duplicate term: " << term.term;
        } else {
            terms.insert(term.term);
        }
    }

    return ret;
}

std::optional<TermData::List> TermData::List::from(const QByteArray& jsonBytes)
{
    auto doc = QJsonDocument::fromJson(jsonBytes);

    if (doc.isNull())
        return std::nullopt;

    return from(doc.object());
}

QJsonObject TermData::List::toQJsonObject() const
{
    QJsonObject obj;
    obj.insert(jsonTools::termsKey, static_cast<QJsonArray>(*this));
    return obj;
}

QJsonArray TermData::List::toQJsonArray() const
{
    QJsonArray arr;

    for (const auto& item : *this)
        arr.push_back(static_cast<QJsonObject>(item));

    return arr;
}

QByteArray TermData::List::toQByteArray() const { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }
