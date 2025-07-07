// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TermValidator.h"

#include <QDateTime>

#include "source/CommonTools/GroupUuid.h"
#include "source/CommonTools/JsonTools.h"
#include "source/CommonTools/TermUuid.h"

import TextModule.TextTools;

TermJsonValidator::TermJsonValidator(bool checkUuid, bool checkLastEdit)
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

TermJsonValidator TermJsonValidator::minimalStaticDataChecks()
{
    TermJsonValidator ret;

    ret.clear();

    ret.addCheck(&validTermDef);

    return ret;
}

TermJsonValidator::CheckResult TermJsonValidator::validUuid(const QJsonObject& obj)
{
    const auto& field = obj[JsonTools::uuidKey];

    if (!field.isString())
        return std::unexpected(ErrorCode::JsonUuidFieldMissedOrWrongType);

    if (auto uuid = TermUuid::from(field.toString()); !uuid.has_value())
        return std::unexpected(ErrorCode::TermUuidInvalid);

    return {};
}

TermJsonValidator::CheckResult TermJsonValidator::validTerm(const QJsonObject& obj)
{
    const auto& field = obj[JsonTools::termKey];

    if (!field.isString())
        return std::unexpected(ErrorCode::JsonTermFieldMissedOrWrongType);

    if (field.toString().isEmpty())
        return std::unexpected(ErrorCode::TermEmpty);

    return {};
}

TermJsonValidator::CheckResult TermJsonValidator::validDefinition(const QJsonObject& obj)
{
    return checkOrError(obj[JsonTools::definitionKey].isString(), ErrorCode::JsonDefinitionFieldMissedOrWrongType);
}

TermJsonValidator::CheckResult TermJsonValidator::validTermDef(const QJsonObject& obj)
{
    auto termDefString = obj[JsonTools::termDefKey].toString();

    if (!TextTools::isTermAndDefinition(termDefString))
        return std::unexpected(ErrorCode::JsonTermDefFieldWrongContentOrType);

    auto [term, _] = TextTools::splitTermAndDefinition(termDefString);

    if (term.isEmpty())
        return std::unexpected(ErrorCode::JsonTermDefFieldWrongContentOrType);

    return {};
}

TermJsonValidator::CheckResult TermJsonValidator::validDescription(const QJsonObject& obj)
{
    return checkOrError(obj[JsonTools::descriptionKey].isString(), ErrorCode::JsonDescriptionFieldMissedOrWrongType);
}

TermJsonValidator::CheckResult TermJsonValidator::validExamples(const QJsonObject& obj)
{
    return checkOrError(obj[JsonTools::examplesKey].isString(), ErrorCode::JsonExamplesFieldMissedOrWrongType);
}

TermJsonValidator::CheckResult TermJsonValidator::validWikiUrl(const QJsonObject& obj)
{
    return checkOrError(obj[JsonTools::wikiUrlKey].isString(), ErrorCode::JsonWikiUrlFieldMissedOrWrongType);
}

TermJsonValidator::CheckResult TermJsonValidator::validWikiImage(const QJsonObject& obj)
{
    return checkOrError(obj[JsonTools::wikiImageKey].isString(), ErrorCode::JsonWikiImageFieldMissedOrWrongType);
}

TermJsonValidator::CheckResult TermJsonValidator::validGroupUuid(const QJsonObject& obj)
{
    const auto& field = obj[JsonTools::groupUuidKey];

    if (!field.isString())
        return std::unexpected(ErrorCode::JsonGroupUuidFieldMissedOrWrongType);

    if (auto uuid = GroupUuid::from(field.toString()); !uuid.has_value())
        return std::unexpected(ErrorCode::GroupUuidInvalid);

    return {};
}

TermJsonValidator::CheckResult TermJsonValidator::validLastEditField(const QJsonObject& obj)
{
    return checkOrError(obj[JsonTools::lastEditKey].isString(), ErrorCode::JsonLastEditFieldMissedOrWrongType);
}

TermJsonValidator::CheckResult TermJsonValidator::validLastEdit(const QJsonObject& obj)
{
    return checkOrError(!QDateTime::fromString(obj[JsonTools::lastEditKey].toString(), Qt::ISODate).isNull(),
                        ErrorCode::LastEditInvalid);
}
