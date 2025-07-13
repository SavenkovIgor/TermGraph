// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/TermDataInterface/GroupValidator.h"

#include <QDateTime>

#include "source/CommonTools/Errors.h"
#include "source/CommonTools/GroupUuid.h"
#include "source/CommonTools/JsonTools.h"

GroupJsonValidator GroupJsonValidator::defaultChecks(bool checkLastEdit)
{
    GroupJsonValidator ret;
    ret.addCheck(&validUuid);
    ret.addCheck(&validName);
    ret.addCheck(&validComment);
    ret.addCheck(&validSizeField);

    if (checkLastEdit) {
        ret.addCheck(&validLastEdit);
    }
    return ret;
}

GroupJsonValidator GroupJsonValidator::fullChecks()
{
    GroupJsonValidator ret;
    ret.addCheck(&validUuid);
    ret.addCheck(&validName);
    ret.addCheck(&validComment);
    ret.addCheck(&validNodesArray);
    return ret;
}

GroupJsonValidator GroupJsonValidator::staticDataChecks()
{
    GroupJsonValidator ret;
    ret.addCheck(&validName);
    ret.addCheck(&validNodesArray);
    return ret;
}
GroupJsonValidator::CheckResult GroupJsonValidator::validUuid(const QJsonObject& obj)
{
    const auto& field = obj[JsonTools::uuidKey];

    if (!field.isString())
        return std::unexpected(ErrorCode::JsonUuidFieldMissedOrWrongType);

    if (auto uuid = GroupUuid::from(field.toString()); !uuid.has_value())
        return std::unexpected(ErrorCode::GroupUuidInvalid);

    return {};
}

GroupJsonValidator::CheckResult GroupJsonValidator::validName(const QJsonObject& obj)
{
    const auto& field = obj[JsonTools::nameKey];

    if (!field.isString())
        return std::unexpected(ErrorCode::JsonNameFieldMissedOrWrongType);

    if (field.toString().isEmpty())
        return std::unexpected(ErrorCode::GroupNameEmpty);

    return {};
}

GroupJsonValidator::CheckResult GroupJsonValidator::validComment(const QJsonObject& obj)
{
    return checkOrError(obj[JsonTools::commentKey].isString(), ErrorCode::JsonCommentFieldMissedOrWrongType);
}

GroupJsonValidator::CheckResult GroupJsonValidator::validSizeField(const QJsonObject& obj)
{
    return checkOrError(obj[JsonTools::sizeKey].isDouble() && obj[JsonTools::sizeKey].toInt(-1) != -1,
                        ErrorCode::JsonSizeFieldMissedOrWrongType);
}

GroupJsonValidator::CheckResult GroupJsonValidator::validLastEdit(const QJsonObject& obj)
{
    const auto& field = obj[JsonTools::lastEditKey];

    if (!field.isString())
        return std::unexpected(ErrorCode::JsonLastEditFieldMissedOrWrongType);

    if (auto dt = QDateTime::fromString(field.toString(), Qt::ISODate); dt.isNull())
        return std::unexpected(ErrorCode::LastEditInvalid);

    return {};
}

GroupJsonValidator::CheckResult GroupJsonValidator::validNodesArray(const QJsonObject& obj)
{
    return checkOrError(obj[JsonTools::termsKey].isArray(), ErrorCode::JsonNodesFieldMissedOrWrongType);
}

GroupJsonValidator::CheckResult GroupJsonValidator::validNodesLastEdit(const QJsonObject& obj)
{
    const auto& field = obj[JsonTools::nodesLastEditKey];

    if (!field.isString())
        return std::unexpected(ErrorCode::JsonNodesLastEditFieldMissedOrWrongType);

    if (auto dt = QDateTime::fromString(field.toString(), Qt::ISODate); dt.isNull())
        return std::unexpected(ErrorCode::NodesLastEditInvalid);

    return {};
}
