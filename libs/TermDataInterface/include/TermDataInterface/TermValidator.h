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

#include <QJsonObject>

#include <CommonTools/GroupUuid.h>
#include <CommonTools/TermUuid.h>
#include <CommonTools/Validator.h>

enum class TermJsonErrors {
    UuidFieldMissedOrWrongType,
    InvalidUuid,

    TermFieldMissedOrWrongType,
    TermIsEmpty,

    DefinitionFieldMissedOrWrongType,
    DescriptionFieldMissedOrWrongType,
    ExamplesFieldMissedOrWrongType,
    WikiUrlFieldMissedOrWrongType,
    WikiImageFieldMissedOrWrongType,

    GroupUuidFieldMissedOrWrongType,
    InvalidGroupUuid,

    LastEditFieldMissedOrWrongType,
    InvalidLastEdit
};

class TermJsonValidator : public Validator<QJsonObject, TermJsonErrors>
{
public:
    constexpr static auto uuidKey        = "uuid";
    constexpr static auto termKey        = "term";
    constexpr static auto definitionKey  = "definition";
    constexpr static auto descriptionKey = "description";
    constexpr static auto examplesKey    = "examples";
    constexpr static auto wikiUrlKey     = "wikiUrl";
    constexpr static auto wikiImageKey   = "wikiImage";
    constexpr static auto groupUuidKey   = "groupUuid";
    constexpr static auto lastEditKey    = "lastEdit";

    TermJsonValidator(bool checkUuid, bool checkLastEdit)
    {
        if (checkUuid) {
            addCheck(&validUuidField, TermJsonErrors::UuidFieldMissedOrWrongType);
            addCheck(&validUuid, TermJsonErrors::InvalidUuid);
        }

        addCheck(&validTermField, TermJsonErrors::TermFieldMissedOrWrongType);
        addCheck(&termNotEmpty, TermJsonErrors::TermIsEmpty);

        addCheck(&validDefinitionField, TermJsonErrors::DefinitionFieldMissedOrWrongType);
        addCheck(&validDescriptionField, TermJsonErrors::DescriptionFieldMissedOrWrongType);
        addCheck(&validExamplesField, TermJsonErrors::ExamplesFieldMissedOrWrongType);
        addCheck(&validWikiUrlField, TermJsonErrors::WikiUrlFieldMissedOrWrongType);
        addCheck(&validWikiImageField, TermJsonErrors::WikiImageFieldMissedOrWrongType);

        addCheck(&validGroupUuidField, TermJsonErrors::GroupUuidFieldMissedOrWrongType);
        addCheck(&validGroupUuid, TermJsonErrors::InvalidGroupUuid);

        if (checkLastEdit) {
            addCheck(&validLastEditField, TermJsonErrors::LastEditFieldMissedOrWrongType);
            addCheck(&validLastEdit, TermJsonErrors::InvalidLastEdit);
        }
    }

private:
    static bool validUuidField(const QJsonObject& obj) { return obj[uuidKey].isString(); }
    static bool validUuid(const QJsonObject& obj) { return TermUuid::create(obj[uuidKey].toString()).has_value(); }

    static bool validTermField(const QJsonObject& obj) { return obj[termKey].isString(); }
    static bool termNotEmpty(const QJsonObject& obj) { return !(obj[termKey].toString().isEmpty()); }

    static bool validDefinitionField(const QJsonObject& obj) { return obj[definitionKey].isString(); }
    static bool validDescriptionField(const QJsonObject& obj) { return obj[descriptionKey].isString(); }
    static bool validExamplesField(const QJsonObject& obj) { return obj[examplesKey].isString(); }
    static bool validWikiUrlField(const QJsonObject& obj) { return obj[wikiUrlKey].isString(); }
    static bool validWikiImageField(const QJsonObject& obj) { return obj[wikiImageKey].isString(); }

    static bool validGroupUuidField(const QJsonObject& obj) { return obj[groupUuidKey].isString(); }
    static bool validGroupUuid(const QJsonObject& obj)
    {
        return GroupUuid::create(obj[groupUuidKey].toString()).has_value();
    }

    static bool validLastEditField(const QJsonObject& obj) { return obj[lastEditKey].isString(); }
    static bool validLastEdit(const QJsonObject& obj)
    {
        return !(QDateTime::fromString(obj[lastEditKey].toString(), Qt::ISODate).isNull());
    }
};
