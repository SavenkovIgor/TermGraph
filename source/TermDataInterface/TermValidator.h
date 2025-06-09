// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QJsonObject>

#include "source/CommonTools/GroupUuid.h"
#include "source/CommonTools/JsonTools.h"
#include "source/CommonTools/TermUuid.h"
#include "source/CommonTools/Validator.h"

#include "source/Text/TextTools.h"

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
            return std::unexpected(ErrorCode::JsonUuidFieldMissedOrWrongType);

        if (auto uuid = TermUuid::from(field.toString()); !uuid.has_value())
            return std::unexpected(ErrorCode::TermUuidInvalid);

        return {};
    }

    static CheckResult validTerm(const QJsonObject& obj)
    {
        const auto& field = obj[JsonTools::termKey];

        if (!field.isString())
            return std::unexpected(ErrorCode::JsonTermFieldMissedOrWrongType);

        if (field.toString().isEmpty())
            return std::unexpected(ErrorCode::TermEmpty);

        return {};
    }

    static CheckResult validDefinition(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::definitionKey].isString(), ErrorCode::JsonDefinitionFieldMissedOrWrongType);
    }

    static CheckResult validTermDef(const QJsonObject& obj)
    {
        auto termDefString = obj[JsonTools::termDefKey].toString();

        if (!TextTools::isTermAndDefinition(termDefString))
            return std::unexpected(ErrorCode::JsonTermDefFieldWrongContentOrType);

        auto [term, _] = TextTools::splitTermAndDefinition(termDefString);

        if (term.isEmpty())
            return std::unexpected(ErrorCode::JsonTermDefFieldWrongContentOrType);

        return {};
    }

    static CheckResult validDescription(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::descriptionKey].isString(), ErrorCode::JsonDescriptionFieldMissedOrWrongType);
    }

    static CheckResult validExamples(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::examplesKey].isString(), ErrorCode::JsonExamplesFieldMissedOrWrongType);
    }

    static CheckResult validWikiUrl(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::wikiUrlKey].isString(), ErrorCode::JsonWikiUrlFieldMissedOrWrongType);
    }

    static CheckResult validWikiImage(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::wikiImageKey].isString(), ErrorCode::JsonWikiImageFieldMissedOrWrongType);
    }

    static CheckResult validGroupUuid(const QJsonObject& obj)
    {
        const auto& field = obj[JsonTools::groupUuidKey];

        if (!field.isString())
            return std::unexpected(ErrorCode::JsonGroupUuidFieldMissedOrWrongType);

        if (auto uuid = GroupUuid::from(field.toString()); !uuid.has_value())
            return std::unexpected(ErrorCode::GroupUuidInvalid);

        return {};
    }

    static CheckResult validLastEditField(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::lastEditKey].isString(), ErrorCode::JsonLastEditFieldMissedOrWrongType);
    }

    static CheckResult validLastEdit(const QJsonObject& obj)
    {
        return checkOrError(!QDateTime::fromString(obj[JsonTools::lastEditKey].toString(), Qt::ISODate).isNull(),
                            ErrorCode::LastEditInvalid);
    }
};
