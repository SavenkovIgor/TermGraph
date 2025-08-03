// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <expected>

#include <QDateTime>
#include <QJsonObject>

#include "source/CommonTools/Errors.h"
#include "source/CommonTools/GroupUuid.h"
#include "source/CommonTools/TermUuid.h"

export module CommonTools.TermJsonValidator;

import CommonTools.JsonTools;
import CommonTools.Validator;
import Text;

using namespace std;

export class TermJsonValidator : public Validator<QJsonObject>
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
        const auto& field = obj[jsonTools::uuidKey];

        if (!field.isString())
            return unexpected(ErrorCode::JsonUuidFieldMissedOrWrongType);

        if (auto uuid = TermUuid::from(field.toString()); !uuid.has_value())
            return unexpected(ErrorCode::TermUuidInvalid);

        return {};
    }

    static CheckResult validTerm(const QJsonObject& obj)
    {
        const auto& field = obj[jsonTools::termKey];

        if (!field.isString())
            return unexpected(ErrorCode::JsonTermFieldMissedOrWrongType);

        if (field.toString().isEmpty())
            return unexpected(ErrorCode::TermEmpty);

        return {};
    }

    static CheckResult validDefinition(const QJsonObject& obj)
    {
        return checkOrError(obj[jsonTools::definitionKey].isString(), ErrorCode::JsonDefinitionFieldMissedOrWrongType);
    }

    static CheckResult validTermDef(const QJsonObject& obj)
    {
        auto termDefString = obj[jsonTools::termDefKey].toString();

        if (!TextTools::isTermAndDefinition(termDefString))
            return unexpected(ErrorCode::JsonTermDefFieldWrongContentOrType);

        auto [term, _] = TextTools::splitTermAndDefinition(termDefString);

        if (term.isEmpty())
            return unexpected(ErrorCode::JsonTermDefFieldWrongContentOrType);

        return {};
    }

    static CheckResult validDescription(const QJsonObject& obj)
    {
        return checkOrError(obj[jsonTools::descriptionKey].isString(), ErrorCode::JsonDescriptionFieldMissedOrWrongType);
    }

    static CheckResult validExamples(const QJsonObject& obj)
    {
        return checkOrError(obj[jsonTools::examplesKey].isString(), ErrorCode::JsonExamplesFieldMissedOrWrongType);
    }

    static CheckResult validWikiUrl(const QJsonObject& obj)
    {
        return checkOrError(obj[jsonTools::wikiUrlKey].isString(), ErrorCode::JsonWikiUrlFieldMissedOrWrongType);
    }

    static CheckResult validWikiImage(const QJsonObject& obj)
    {
        return checkOrError(obj[jsonTools::wikiImageKey].isString(), ErrorCode::JsonWikiImageFieldMissedOrWrongType);
    }

    static CheckResult validGroupUuid(const QJsonObject& obj)
    {
        const auto& field = obj[jsonTools::groupUuidKey];

        if (!field.isString())
            return unexpected(ErrorCode::JsonGroupUuidFieldMissedOrWrongType);

        if (auto uuid = GroupUuid::from(field.toString()); !uuid.has_value())
            return unexpected(ErrorCode::GroupUuidInvalid);

        return {};
    }

    static CheckResult validLastEditField(const QJsonObject& obj)
    {
        return checkOrError(obj[jsonTools::lastEditKey].isString(), ErrorCode::JsonLastEditFieldMissedOrWrongType);
    }

    static CheckResult validLastEdit(const QJsonObject& obj)
    {
        return checkOrError(!QDateTime::fromString(obj[jsonTools::lastEditKey].toString(), Qt::ISODate).isNull(),
                            ErrorCode::LastEditInvalid);
    }
};
