// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QJsonObject>

#include <CommonTools/GroupUuid.h>
#include <CommonTools/JsonTools.h>
#include <CommonTools/TermUuid.h>
#include <CommonTools/Validator.h>

#include <Text/TextTools.h>

class TermJsonValidator : public Validator<QJsonObject>
{
public:
    TermJsonValidator(bool checkUuid = true, bool checkLastEdit = true)
    {
        if (checkUuid) {
            addCheck(&validUuid);
        }

        addCheck(&validTerm);

        addCheck(&validDefinition);
        addCheck(&validDescription);
        addCheck(&validExamples);
        addCheck(&validWikiUrl);
        addCheck(&validWikiImage);

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

        ret.addCheck(&validTermDef);

        return ret;
    }

private:
    static CheckResult validUuid(const QJsonObject& obj)
    {
        const auto& field = obj[JsonTools::uuidKey];

        if (!field.isString())
            return ErrorCodes::JsonUuidFieldMissedOrWrongType;

        if (auto uuid = TermUuid::from(field.toString()); !uuid.has_value())
            return ErrorCodes::TermUuidInvalid;

        return std::nullopt;
    }

    static CheckResult validTerm(const QJsonObject& obj)
    {
        const auto& field = obj[JsonTools::termKey];

        if (!field.isString())
            return ErrorCodes::JsonTermFieldMissedOrWrongType;

        if (field.toString().isEmpty())
            return ErrorCodes::TermEmpty;

        return std::nullopt;
    }

    static CheckResult validDefinition(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::definitionKey].isString(), ErrorCodes::JsonDefinitionFieldMissedOrWrongType);
    }

    static CheckResult validTermDef(const QJsonObject& obj)
    {
        auto termDefString = obj[JsonTools::termDefKey].toString();

        if (!TextTools::isTermAndDefinition(termDefString))
            return ErrorCodes::JsonTermDefFieldWrongContentOrType;

        auto [term, _] = TextTools::splitTermAndDefinition(termDefString);

        if (term.isEmpty())
            return ErrorCodes::JsonTermDefFieldWrongContentOrType;

        return std::nullopt;
    }

    static CheckResult validDescription(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::descriptionKey].isString(),
                            ErrorCodes::JsonDescriptionFieldMissedOrWrongType);
    }

    static CheckResult validExamples(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::examplesKey].isString(), ErrorCodes::JsonExamplesFieldMissedOrWrongType);
    }

    static CheckResult validWikiUrl(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::wikiUrlKey].isString(), ErrorCodes::JsonWikiUrlFieldMissedOrWrongType);
    }

    static CheckResult validWikiImage(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::wikiImageKey].isString(), ErrorCodes::JsonWikiImageFieldMissedOrWrongType);
    }

    static CheckResult validGroupUuid(const QJsonObject& obj)
    {
        const auto& field = obj[JsonTools::groupUuidKey];

        if (!field.isString())
            return ErrorCodes::JsonGroupUuidFieldMissedOrWrongType;

        if (auto uuid = GroupUuid::from(field.toString()); !uuid.has_value())
            return ErrorCodes::GroupUuidInvalid;

        return std::nullopt;
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
