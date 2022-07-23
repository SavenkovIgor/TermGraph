// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <optional>
#include <vector>

#include <QByteArray>
#include <QDateTime>
#include <QJsonObject>
#include <QString>
#include <QUuid>

#include <CommonTools/GroupUuid.h>
#include <CommonTools/HandyTypes.h>
#include <CommonTools/JsonTools.h>
#include <CommonTools/TermUuid.h>
#include <TermDataInterface/TermValidator.h>

// TODO: Check tests!
// TODO: Make class and make fields private
struct TermData
{
    Opt<TermUuid> uuid;
    QString       term;
    QString       definition;
    QString       description;
    QString       examples;
    QString       wikiUrl;
    QString       wikiImage;
    GroupUuid     groupUuid;
    QDateTime     lastEdit;

    inline bool isNull() const { return uuid.has_value() && term.isEmpty(); }

    inline bool operator==(const TermData& rhs) const
    {
        if (uuid != rhs.uuid)
            return false;

        if (term != rhs.term)
            return false;

        if (definition != rhs.definition)
            return false;

        if (description != rhs.description)
            return false;

        if (examples != rhs.examples)
            return false;

        if (wikiUrl != rhs.wikiUrl)
            return false;

        if (wikiImage != rhs.wikiImage)
            return false;

        if (groupUuid != rhs.groupUuid)
            return false;

        if (lastEdit != rhs.lastEdit)
            return false;

        return true;
    }

    enum class JsonCheckMode { Import, AddTerm, UpdateTerm };

    // --- JSON ---
    // Returns valid object or nullopt
    static inline Opt<TermData> create(const QJsonObject& obj, JsonCheckMode mode)
    {
        bool checkUuid     = true;
        bool checkLastEdit = true;

        if (mode == JsonCheckMode::AddTerm) {
            checkUuid     = false;
            checkLastEdit = false;
        }

        if (!TermJsonValidator(checkUuid, checkLastEdit).check(obj))
            return std::nullopt;

        auto gUuid = GroupUuid::create(obj[TermJsonValidator::groupUuidKey].toString());

        if (!gUuid)
            return std::nullopt;

        TermData ret{
            .uuid        = TermUuid::create(obj[TermJsonValidator::uuidKey].toString()),
            .term        = obj[TermJsonValidator::termKey].toString(),
            .definition  = obj[TermJsonValidator::definitionKey].toString(),
            .description = obj[TermJsonValidator::descriptionKey].toString(),
            .examples    = obj[TermJsonValidator::examplesKey].toString(),
            .wikiUrl     = obj[TermJsonValidator::wikiUrlKey].toString(),
            .wikiImage   = obj[TermJsonValidator::wikiImageKey].toString(),
            .groupUuid   = *gUuid,
            .lastEdit    = QDateTime::fromString(obj[TermJsonValidator::lastEditKey].toString(), Qt::ISODate),
        };

        if (ret.isNull()) // Release safety
            return std::nullopt;

        return ret;
    }

    static inline Opt<TermData> create(const QByteArray& jsonBytes, JsonCheckMode mode)
    {
        auto doc = QJsonDocument::fromJson(jsonBytes);

        if (doc.isNull())
            return std::nullopt;

        return create(doc.object(), mode);
    }

    operator QJsonObject() const
    {
        QJsonObject ret;

        ret.insert(TermJsonValidator::uuidKey, (uuid ? uuid->toString() : ""));
        ret.insert(TermJsonValidator::termKey, term);
        ret.insert(TermJsonValidator::definitionKey, definition);
        ret.insert(TermJsonValidator::descriptionKey, description);
        ret.insert(TermJsonValidator::examplesKey, examples);
        ret.insert(TermJsonValidator::wikiUrlKey, wikiUrl);
        ret.insert(TermJsonValidator::wikiImageKey, wikiImage);
        ret.insert(TermJsonValidator::groupUuidKey, groupUuid.toString());
        ret.insert(TermJsonValidator::lastEditKey, lastEdit.toString(Qt::ISODate));

        return ret;
    }

    explicit operator QByteArray() const { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }

    class List : public std::vector<TermData>
    {
    public:
        static inline Opt<List> create(const QJsonObject& obj)
        {
            List ret;

            if (!obj[JsonTools::termsKey].isArray())
                return std::nullopt;

            for (const auto& termJson : obj[JsonTools::termsKey].toArray()) {
                if (auto termData = TermData::create(termJson.toObject(), JsonCheckMode::Import)) {
                    ret.push_back(*termData);
                } else {
                    qWarning("Wrong groupData in received data");
                }
            }

            return ret;
        }

        static inline Opt<List> create(const QByteArray& jsonBytes)
        {
            auto doc = QJsonDocument::fromJson(jsonBytes);

            if (doc.isNull())
                return std::nullopt;

            return create(doc.object());
        }

        explicit operator QJsonObject() const
        {
            QJsonArray arr;

            for (const auto& item : *this)
                arr.push_back(static_cast<QJsonObject>(item));

            QJsonObject obj;
            obj.insert(JsonTools::termsKey, arr);
            return obj;
        }

        explicit operator QByteArray() const { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }
    };
};
