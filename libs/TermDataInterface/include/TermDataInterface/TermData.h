/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <optional>
#include <vector>

#include <QDateTime>
#include <QJsonObject>
#include <QString>
#include <QUuid>

#include <CommonTools/HandyTypes.h>
#include <TermDataInterface/TermValidator.h>

// TODO: Check tests!
struct TermData
{
    using List = std::vector<TermData>;

    QUuid     uuid;
    QString   term;
    QString   definition;
    QString   description;
    QString   examples;
    QString   wikiUrl;
    QString   wikiImage;
    QUuid     groupUuid;
    QDateTime lastEdit;

    inline bool isNull() const { return uuid.isNull() && term.isEmpty(); }

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

    enum class JsonCheckMode { Import, AddNode, UpdateNode };

    // --- JSON ---
    // Returns valid object or nullopt
    static inline Opt<TermData> fromJson(QJsonObject obj, JsonCheckMode mode)
    {
        bool checkUuid     = true;
        bool checkLastEdit = true;

        if (mode == JsonCheckMode::AddNode)
            checkLastEdit = false;

        if (!TermJsonValidator(checkUuid, checkLastEdit).check(obj))
            return std::nullopt;

        TermData ret;

        ret.uuid        = QUuid(obj[TermJsonValidator::uuidKey].toString());
        ret.term        = obj[TermJsonValidator::termKey].toString();
        ret.definition  = obj[TermJsonValidator::definitionKey].toString();
        ret.description = obj[TermJsonValidator::descriptionKey].toString();
        ret.examples    = obj[TermJsonValidator::examplesKey].toString();
        ret.wikiUrl     = obj[TermJsonValidator::wikiUrlKey].toString();
        ret.wikiImage   = obj[TermJsonValidator::wikiImageKey].toString();
        ret.groupUuid   = QUuid(obj[TermJsonValidator::groupUuidKey].toString());
        ret.lastEdit    = QDateTime::fromString(obj[TermJsonValidator::lastEditKey].toString(), Qt::ISODate);

        if (ret.isNull()) // Release safety
            return std::nullopt;

        return ret;
    }

    operator QJsonObject()
    {
        QJsonObject ret;

        ret.insert(TermJsonValidator::uuidKey, uuid.toString());
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

    explicit operator QString() { return QString(QJsonDocument(static_cast<QJsonObject>(*this)).toJson()); }
};
