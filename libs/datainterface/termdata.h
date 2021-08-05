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

    // --- JSON ---
    // Returns valid object or nullopt
    static inline bool isValidJson(const QJsonObject& obj)
    {
        bool ret = true;

        ret &= obj[uuidKey].isString();
        ret &= !QUuid(obj[uuidKey].toString()).isNull();

        ret &= obj[termKey].isString();
        ret &= !obj[termKey].toString().isEmpty();

        ret &= obj[definitionKey].isString();
        ret &= obj[descriptionKey].isString();
        ret &= obj[examplesKey].isString();
        ret &= obj[wikiUrlKey].isString();
        ret &= obj[wikiImageKey].isString();

        ret &= obj[groupUuidKey].isString();
        ret &= !QUuid(obj[groupUuidKey].toString()).isNull();

        ret &= obj[lastEditKey].isString();
        ret &= !QDateTime::fromString(obj[lastEditKey].toString(), Qt::ISODate).isNull();

        return ret;
    }

    static inline std::optional<TermData> fromJson(QJsonObject obj)
    {
        if (!isValidJson(obj))
            return std::nullopt;

        TermData ret;

        ret.uuid        = QUuid(obj[uuidKey].toString());
        ret.term        = obj[termKey].toString();
        ret.definition  = obj[definitionKey].toString();
        ret.description = obj[descriptionKey].toString();
        ret.examples    = obj[examplesKey].toString();
        ret.wikiUrl     = obj[wikiUrlKey].toString();
        ret.wikiImage   = obj[wikiImageKey].toString();
        ret.groupUuid   = QUuid(obj[groupUuidKey].toString());
        ret.lastEdit    = QDateTime::fromString(obj[lastEditKey].toString(), Qt::ISODate);

        assert(!ret.isNull());

        if (ret.isNull()) // Release safety
            return std::nullopt;

        return ret;
    }

    inline QJsonObject toJson() const
    {
        QJsonObject ret;

        ret.insert(uuidKey, uuid.toString());
        ret.insert(termKey, term);
        ret.insert(definitionKey, definition);
        ret.insert(descriptionKey, description);
        ret.insert(examplesKey, examples);
        ret.insert(wikiUrlKey, wikiUrl);
        ret.insert(wikiImageKey, wikiImage);
        ret.insert(groupUuidKey, groupUuid.toString());
        ret.insert(lastEditKey, lastEdit.toString(Qt::ISODate));

        return ret;
    }

private:
    constexpr static auto uuidKey        = "uuid";
    constexpr static auto termKey        = "term";
    constexpr static auto definitionKey  = "definition";
    constexpr static auto descriptionKey = "description";
    constexpr static auto examplesKey    = "examples";
    constexpr static auto wikiUrlKey     = "wikiUrl";
    constexpr static auto wikiImageKey   = "wikiImage";
    constexpr static auto groupUuidKey   = "groupUuid";
    constexpr static auto lastEditKey    = "lastEdit";
};
