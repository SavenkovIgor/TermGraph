// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/TermDataInterface/TermData.h"

#include <QJsonDocument>

#include <set>

import CommonTools.JsonKeys;
import CommonTools.JsonTools;
import CommonTools.TermJsonValidator;
import Text;

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
        gUuid = GroupUuid::from(obj[jsonKeys::groupUuidKey].toString());
    }

    if (!gUuid)
        return std::nullopt;

    QDateTime lastEdit;

    if (obj.contains(jsonKeys::lastEditKey)) {
        lastEdit = QDateTime::fromString(obj[jsonKeys::lastEditKey].toString(), Qt::ISODate);
    }

    std::optional<TermUuid> uuid;

    if (obj.contains(jsonKeys::uuidKey)) {
        uuid = TermUuid::from(obj[jsonKeys::uuidKey].toString());
    }

    QString term;
    QString definition;

    if (mode == JsonCheckMode::Minimal) {
        auto termDef = obj[jsonKeys::termDefKey].toString();

        auto res = TextTools::splitTermAndDefinition(termDef);

        term       = res.first;
        definition = res.second;
    } else {
        term       = obj[jsonKeys::termKey].toString();
        definition = obj[jsonKeys::definitionKey].toString();
    }

    TermData ret{
        .uuid          = uuid,
        .term          = term,
        .definition    = definition,
        .description   = obj[jsonKeys::descriptionKey].toString(""),
        .examples      = obj[jsonKeys::examplesKey].toString(""),
        .wikiUrl       = obj[jsonKeys::wikiUrlKey].toString(""),
        .wikiImage     = obj[jsonKeys::wikiImageKey].toString(""),
        .knowledgeArea = obj[jsonKeys::knowledgeAreaKey].toString(""),
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

    ret.insert(jsonKeys::uuidKey, (uuid ? uuid->toString() : ""));
    ret.insert(jsonKeys::termKey, term);
    ret.insert(jsonKeys::definitionKey, definition);

    ret = jsonTools::addIfNotEmpty(ret, jsonKeys::descriptionKey, description);
    ret = jsonTools::addIfNotEmpty(ret, jsonKeys::examplesKey, examples);
    ret = jsonTools::addIfNotEmpty(ret, jsonKeys::wikiUrlKey, wikiUrl);
    ret = jsonTools::addIfNotEmpty(ret, jsonKeys::wikiImageKey, wikiImage);
    ret = jsonTools::addIfNotEmpty(ret, jsonKeys::knowledgeAreaKey, knowledgeArea);

    ret.insert(jsonKeys::groupUuidKey, groupUuid.toString());
    ret.insert(jsonKeys::lastEditKey, lastEdit.toString(Qt::ISODate));

    return ret;
}

QJsonObject TermData::toMinimalQJsonObject() const
{
    QJsonObject ret;

    auto termDef = TextTools::joinTermDef(term, definition);

    ret.insert(jsonKeys::termDefKey, termDef);

    ret = jsonTools::addIfNotEmpty(ret, jsonKeys::descriptionKey, description);
    ret = jsonTools::addIfNotEmpty(ret, jsonKeys::examplesKey, examples);
    ret = jsonTools::addIfNotEmpty(ret, jsonKeys::wikiUrlKey, wikiUrl);
    ret = jsonTools::addIfNotEmpty(ret, jsonKeys::wikiImageKey, wikiImage);
    ret = jsonTools::addIfNotEmpty(ret, jsonKeys::knowledgeAreaKey, knowledgeArea);

    return ret;
}

QByteArray TermData::toQByteArray() const { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }

std::optional<TermData::List> TermData::List::from(const QJsonObject& obj)
{
    TermData::List ret;

    if (!obj[jsonKeys::termsKey].isArray())
        return std::nullopt;

    return from(obj[jsonKeys::termsKey].toArray());
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
    obj.insert(jsonKeys::termsKey, static_cast<QJsonArray>(*this));
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
