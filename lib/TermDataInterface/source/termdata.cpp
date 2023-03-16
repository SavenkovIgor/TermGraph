// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "include/TermDataInterface/TermData.h"

Opt<TermData> TermData::from(const QJsonObject& obj, JsonCheckMode mode)
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

    Opt<GroupUuid> gUuid;

    if (mode == JsonCheckMode::Minimal) {
        gUuid = GroupUuid::generate();
    } else {
        gUuid = GroupUuid::from(obj[JsonTools::groupUuidKey].toString());
    }

    if (!gUuid)
        return std::nullopt;

    QDateTime lastEdit;

    if (obj.contains(JsonTools::lastEditKey)) {
        lastEdit = QDateTime::fromString(obj[JsonTools::lastEditKey].toString(), Qt::ISODate);
    }

    Opt<TermUuid> uuid;

    if (obj.contains(JsonTools::uuidKey)) {
        uuid = TermUuid::from(obj[JsonTools::uuidKey].toString());
    }

    QString term;
    QString definition;

    if (mode == JsonCheckMode::Minimal) {
        auto termDef = obj[JsonTools::termDefKey].toString();

        auto split = termDef.split(JsonTools::termDefSeparator, Qt::KeepEmptyParts);
        assert(split.size() == 2);

        term       = split[0];
        definition = split[1];
    } else {
        term       = obj[JsonTools::termKey].toString();
        definition = obj[JsonTools::definitionKey].toString();
    }

    TermData ret{
        .uuid        = uuid,
        .term        = term,
        .definition  = definition,
        .description = obj[JsonTools::descriptionKey].toString(""),
        .examples    = obj[JsonTools::examplesKey].toString(""),
        .wikiUrl     = obj[JsonTools::wikiUrlKey].toString(""),
        .wikiImage   = obj[JsonTools::wikiImageKey].toString(""),
        .groupUuid   = *gUuid,
        .lastEdit    = lastEdit,
    };

    if (ret.isNull()) // Release safety
        return std::nullopt;

    return ret;
}

Opt<TermData> TermData::from(const QByteArray& jsonBytes, JsonCheckMode mode)
{
    auto doc = QJsonDocument::fromJson(jsonBytes);

    if (doc.isNull())
        return std::nullopt;

    return from(doc.object(), mode);
}

QJsonObject TermData::toQJsonObject() const
{
    QJsonObject ret;

    ret.insert(JsonTools::uuidKey, (uuid ? uuid->toString() : ""));
    ret.insert(JsonTools::termKey, term);
    ret.insert(JsonTools::definitionKey, definition);

    ret = JsonTools::addIfNotEmpty(ret, JsonTools::descriptionKey, description);
    ret = JsonTools::addIfNotEmpty(ret, JsonTools::examplesKey, examples);
    ret = JsonTools::addIfNotEmpty(ret, JsonTools::wikiUrlKey, wikiUrl);
    ret = JsonTools::addIfNotEmpty(ret, JsonTools::wikiImageKey, wikiImage);

    ret.insert(JsonTools::groupUuidKey, groupUuid.toString());
    ret.insert(JsonTools::lastEditKey, lastEdit.toString(Qt::ISODate));

    return ret;
}

QJsonObject TermData::toMinimalQJsonObject() const
{
    QJsonObject ret;

    auto termDef = term + JsonTools::termDefSeparator + definition;

    ret.insert(JsonTools::termDefKey, termDef);

    ret = JsonTools::addIfNotEmpty(ret, JsonTools::descriptionKey, description);
    ret = JsonTools::addIfNotEmpty(ret, JsonTools::examplesKey, examples);
    ret = JsonTools::addIfNotEmpty(ret, JsonTools::wikiUrlKey, wikiUrl);
    ret = JsonTools::addIfNotEmpty(ret, JsonTools::wikiImageKey, wikiImage);

    return ret;
}

QByteArray TermData::toQByteArray() const { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }

Opt<TermData::List> TermData::List::from(const QJsonObject& obj)
{
    TermData::List ret;

    if (!obj[JsonTools::termsKey].isArray())
        return std::nullopt;

    return from(obj[JsonTools::termsKey].toArray());
}

TermData::List TermData::List::from(const QJsonArray& json, TermData::JsonCheckMode mode)
{
    TermData::List ret;

    for (const auto& termJson : json) {
        if (auto termData = TermData::from(termJson.toObject(), mode)) {
            ret.push_back(*termData);
        } else {
            qWarning("Wrong termData json array");
        }
    }

    return ret;
}

Opt<TermData::List> TermData::List::from(const QByteArray& jsonBytes)
{
    auto doc = QJsonDocument::fromJson(jsonBytes);

    if (doc.isNull())
        return std::nullopt;

    return from(doc.object());
}

QJsonObject TermData::List::toQJsonObject() const
{
    QJsonObject obj;
    obj.insert(JsonTools::termsKey, static_cast<QJsonArray>(*this));
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