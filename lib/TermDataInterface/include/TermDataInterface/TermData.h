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

    inline bool operator==(const TermData& rhs) const = default;

    enum class JsonCheckMode { Import, AddTerm, UpdateTerm };

    // --- JSON ---
    // Returns valid object or nullopt
    static inline Opt<TermData> from(const QJsonObject& obj, JsonCheckMode mode)
    {
        bool checkUuid     = true;
        bool checkLastEdit = true;

        if (mode == JsonCheckMode::AddTerm) {
            checkUuid     = false;
            checkLastEdit = false;
        }

        if (!TermJsonValidator(checkUuid, checkLastEdit).check(obj))
            return std::nullopt;

        auto gUuid = GroupUuid::from(obj[JsonTools::groupUuidKey].toString());

        if (!gUuid)
            return std::nullopt;

        TermData ret{
            .uuid        = TermUuid::from(obj[JsonTools::uuidKey].toString()),
            .term        = obj[JsonTools::termKey].toString(),
            .definition  = obj[JsonTools::definitionKey].toString(),
            .description = obj[JsonTools::descriptionKey].toString(),
            .examples    = obj[JsonTools::examplesKey].toString(),
            .wikiUrl     = obj[JsonTools::wikiUrlKey].toString(),
            .wikiImage   = obj[JsonTools::wikiImageKey].toString(),
            .groupUuid   = *gUuid,
            .lastEdit    = QDateTime::fromString(obj[JsonTools::lastEditKey].toString(), Qt::ISODate),
        };

        if (ret.isNull()) // Release safety
            return std::nullopt;

        return ret;
    }

    static inline Opt<TermData> from(const QByteArray& jsonBytes, JsonCheckMode mode)
    {
        auto doc = QJsonDocument::fromJson(jsonBytes);

        if (doc.isNull())
            return std::nullopt;

        return from(doc.object(), mode);
    }

    operator QJsonObject() const
    {
        QJsonObject ret;

        ret.insert(JsonTools::uuidKey, (uuid ? uuid->toString() : ""));
        ret.insert(JsonTools::termKey, term);
        ret.insert(JsonTools::definitionKey, definition);
        ret.insert(JsonTools::descriptionKey, description);
        ret.insert(JsonTools::examplesKey, examples);
        ret.insert(JsonTools::wikiUrlKey, wikiUrl);
        ret.insert(JsonTools::wikiImageKey, wikiImage);
        ret.insert(JsonTools::groupUuidKey, groupUuid.toString());
        ret.insert(JsonTools::lastEditKey, lastEdit.toString(Qt::ISODate));

        return ret;
    }

    explicit operator QByteArray() const { return QJsonDocument(static_cast<QJsonObject>(*this)).toJson(); }

    class List : public std::vector<TermData>
    {
    public:
        static inline Opt<List> from(const QJsonObject& obj)
        {
            List ret;

            if (!obj[JsonTools::termsKey].isArray())
                return std::nullopt;

            const auto termsArray = obj[JsonTools::termsKey].toArray();

            for (const auto& termJson : termsArray) {
                if (auto termData = TermData::from(termJson.toObject(), JsonCheckMode::Import)) {
                    ret.push_back(*termData);
                } else {
                    qWarning("Wrong groupInfo in received data");
                }
            }

            return ret;
        }

        static inline Opt<List> from(const QByteArray& jsonBytes)
        {
            auto doc = QJsonDocument::fromJson(jsonBytes);

            if (doc.isNull())
                return std::nullopt;

            return from(doc.object());
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
