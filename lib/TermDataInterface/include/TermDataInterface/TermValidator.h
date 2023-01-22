// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QJsonObject>

#include <CommonTools/GroupUuid.h>
#include <CommonTools/JsonTools.h>
#include <CommonTools/TermUuid.h>
#include <CommonTools/Validator.h>

class TermJsonValidator : public Validator<QJsonObject, ErrorCodes>
{
public:
    TermJsonValidator(bool checkUuid, bool checkLastEdit)
    {
        if (checkUuid) {
            addCheck(&validUuidField, ErrorCodes::JsonUuidFieldMissedOrWrongType);
            addCheck(&validUuid, ErrorCodes::TermUuidInvalid);
        }

        addCheck(&validTermField, ErrorCodes::JsonTermFieldMissedOrWrongType);
        addCheck(&termNotEmpty, ErrorCodes::TermEmpty);

        addCheck(&validDefinitionField, ErrorCodes::JsonDefinitionFieldMissedOrWrongType);
        addCheck(&validDescriptionField, ErrorCodes::JsonDescriptionFieldMissedOrWrongType);
        addCheck(&validExamplesField, ErrorCodes::JsonExamplesFieldMissedOrWrongType);
        addCheck(&validWikiUrlField, ErrorCodes::JsonWikiUrlFieldMissedOrWrongType);
        addCheck(&validWikiImageField, ErrorCodes::JsonWikiImageFieldMissedOrWrongType);

        addCheck(&validGroupUuidField, ErrorCodes::JsonGroupUuidFieldMissedOrWrongType);
        addCheck(&validGroupUuid, ErrorCodes::GroupUuidInvalid);

        if (checkLastEdit) {
            addCheck(&validLastEditField, ErrorCodes::JsonLastEditFieldMissedOrWrongType);
            addCheck(&validLastEdit, ErrorCodes::LastEditInvalid);
        }
    }

private:
    static bool validUuidField(const QJsonObject& obj) { return obj[JsonTools::uuidKey].isString(); }
    static bool validUuid(const QJsonObject& obj)
    {
        return TermUuid::create(obj[JsonTools::uuidKey].toString()).has_value();
    }

    static bool validTermField(const QJsonObject& obj) { return obj[JsonTools::termKey].isString(); }
    static bool termNotEmpty(const QJsonObject& obj) { return !(obj[JsonTools::termKey].toString().isEmpty()); }

    static bool validDefinitionField(const QJsonObject& obj) { return obj[JsonTools::definitionKey].isString(); }
    static bool validDescriptionField(const QJsonObject& obj) { return obj[JsonTools::descriptionKey].isString(); }
    static bool validExamplesField(const QJsonObject& obj) { return obj[JsonTools::examplesKey].isString(); }
    static bool validWikiUrlField(const QJsonObject& obj) { return obj[JsonTools::wikiUrlKey].isString(); }
    static bool validWikiImageField(const QJsonObject& obj) { return obj[JsonTools::wikiImageKey].isString(); }

    static bool validGroupUuidField(const QJsonObject& obj) { return obj[JsonTools::groupUuidKey].isString(); }
    static bool validGroupUuid(const QJsonObject& obj)
    {
        return GroupUuid::create(obj[JsonTools::groupUuidKey].toString()).has_value();
    }

    static bool validLastEditField(const QJsonObject& obj) { return obj[JsonTools::lastEditKey].isString(); }
    static bool validLastEdit(const QJsonObject& obj)
    {
        return !(QDateTime::fromString(obj[JsonTools::lastEditKey].toString(), Qt::ISODate).isNull());
    }
};
