// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QJsonObject>

#include "source/CommonTools/Errors.h"
#include "source/CommonTools/GroupUuid.h"
#include "source/CommonTools/JsonTools.h"
#include "source/CommonTools/Validator.h"

class GroupJsonValidator : public Validator<QJsonObject>
{
public:
    static GroupJsonValidator defaultChecks(bool checkLastEdit = false)
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

    static GroupJsonValidator fullChecks()
    {
        GroupJsonValidator ret;
        ret.addCheck(&validUuid);
        ret.addCheck(&validName);
        ret.addCheck(&validComment);
        ret.addCheck(&validNodesArray);
        return ret;
    }

    static GroupJsonValidator staticDataChecks()
    {
        GroupJsonValidator ret;
        ret.addCheck(&validName);
        ret.addCheck(&validNodesArray);
        return ret;
    }

private:
    static CheckResult validUuid(const QJsonObject& obj)
    {
        const auto& field = obj[JsonTools::uuidKey];

        if (!field.isString())
            return ErrorCodes::JsonUuidFieldMissedOrWrongType;

        if (auto uuid = GroupUuid::from(field.toString()); !uuid.has_value())
            return ErrorCodes::GroupUuidInvalid;

        return std::nullopt;
    }

    static CheckResult validName(const QJsonObject& obj)
    {
        const auto& field = obj[JsonTools::nameKey];

        if (!field.isString())
            return ErrorCodes::JsonNameFieldMissedOrWrongType;

        if (field.toString().isEmpty())
            return ErrorCodes::GroupNameEmpty;

        return std::nullopt;
    }

    static CheckResult validComment(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::commentKey].isString(), ErrorCodes::JsonCommentFieldMissedOrWrongType);
    }

    static CheckResult validSizeField(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::sizeKey].isDouble() && obj[JsonTools::sizeKey].toInt(-1) != -1,
                            ErrorCodes::JsonSizeFieldMissedOrWrongType);
    }

    static CheckResult validLastEdit(const QJsonObject& obj)
    {
        const auto& field = obj[JsonTools::lastEditKey];

        if (!field.isString())
            return ErrorCodes::JsonLastEditFieldMissedOrWrongType;

        if (auto dt = QDateTime::fromString(field.toString(), Qt::ISODate); dt.isNull())
            return ErrorCodes::LastEditInvalid;

        return std::nullopt;
    }

    static CheckResult validNodesArray(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::termsKey].isArray(), ErrorCodes::JsonNodesFieldMissedOrWrongType);
    }

    static CheckResult validNodesLastEdit(const QJsonObject& obj)
    {
        const auto& field = obj[JsonTools::nodesLastEditKey];

        if (!field.isString())
            return ErrorCodes::JsonNodesLastEditFieldMissedOrWrongType;

        if (auto dt = QDateTime::fromString(field.toString(), Qt::ISODate); dt.isNull())
            return ErrorCodes::NodesLastEditInvalid;

        return std::nullopt;
    }
};
