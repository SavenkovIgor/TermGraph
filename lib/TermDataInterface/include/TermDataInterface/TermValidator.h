// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QJsonObject>

#include <CommonTools/GroupUuid.h>
#include <CommonTools/JsonTools.h>
#include <CommonTools/TermUuid.h>
#include <CommonTools/Validator.h>

class TermJsonValidator : public Validator<QJsonObject>
{
public:
    TermJsonValidator(bool checkUuid = true, bool checkLastEdit = true)
    {
        if (checkUuid) {
            addCheck(&validUuidField);
            addCheck(&validUuid);
        }

        addCheck(&validTermField);
        addCheck(&termNotEmpty);

        addCheck(&validDefinitionField);
        addCheck(&validDescriptionField);
        addCheck(&validExamplesField);
        addCheck(&validWikiUrlField);
        addCheck(&validWikiImageField);

        addCheck(&validGroupUuidField);
        addCheck(&validGroupUuid);

        if (checkLastEdit) {
            addCheck(&validLastEditField);
            addCheck(&validLastEdit);
        }
    }

    static TermJsonValidator minimalStaticDataChecks()
    {
        TermJsonValidator ret;

        ret.clear();

        ret.addCheck(&validTermDefFields);

        return ret;
    }

private:
    static CheckResult validUuidField(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::uuidKey].isString(), ErrorCodes::JsonUuidFieldMissedOrWrongType);
    }

    static CheckResult validUuid(const QJsonObject& obj)
    {
        return checkOrError(TermUuid::from(obj[JsonTools::uuidKey].toString()).has_value(), ErrorCodes::TermUuidInvalid);
    }

    static CheckResult validTermField(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::termKey].isString(), ErrorCodes::JsonTermFieldMissedOrWrongType);
    }

    static CheckResult termNotEmpty(const QJsonObject& obj)
    {
        return checkOrError(!(obj[JsonTools::termKey].toString().isEmpty()), ErrorCodes::TermEmpty);
    }

    static CheckResult validDefinitionField(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::definitionKey].isString(), ErrorCodes::JsonDefinitionFieldMissedOrWrongType);
    }

    static CheckResult validTermDefFields(const QJsonObject& obj)
    {
        auto termDefString = obj[JsonTools::termDefKey].toString();

        if (!termDefString.contains(JsonTools::termDefSeparator)) {
            return ErrorCodes::JsonTermDefFieldWrongContentOrType;
        }

        auto termDefSplit = termDefString.split(JsonTools::termDefSeparator, Qt::KeepEmptyParts);

        if (termDefSplit.size() != 2) {
            return ErrorCodes::JsonTermDefFieldWrongContentOrType;
        }

        if (termDefSplit[0].isEmpty()) {
            return ErrorCodes::JsonTermDefFieldWrongContentOrType;
        }

        return std::nullopt;
    }

    static CheckResult validDescriptionField(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::descriptionKey].isString(),
                            ErrorCodes::JsonDescriptionFieldMissedOrWrongType);
    }

    static CheckResult validExamplesField(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::examplesKey].isString(), ErrorCodes::JsonExamplesFieldMissedOrWrongType);
    }

    static CheckResult validWikiUrlField(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::wikiUrlKey].isString(), ErrorCodes::JsonWikiUrlFieldMissedOrWrongType);
    }

    static CheckResult validWikiImageField(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::wikiImageKey].isString(), ErrorCodes::JsonWikiImageFieldMissedOrWrongType);
    }

    static CheckResult validGroupUuidField(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::groupUuidKey].isString(), ErrorCodes::JsonGroupUuidFieldMissedOrWrongType);
    }

    static CheckResult validGroupUuid(const QJsonObject& obj)
    {
        return checkOrError(GroupUuid::from(obj[JsonTools::groupUuidKey].toString()).has_value(),
                            ErrorCodes::GroupUuidInvalid);
    }

    static CheckResult validLastEditField(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::lastEditKey].isString(), ErrorCodes::JsonLastEditFieldMissedOrWrongType);
    }

    static CheckResult validLastEdit(const QJsonObject& obj)
    {
        return checkOrError(!QDateTime::fromString(obj[JsonTools::lastEditKey].toString(), Qt::ISODate).isNull(),
                            ErrorCodes::LastEditInvalid);
    }
};
