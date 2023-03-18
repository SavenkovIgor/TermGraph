// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QJsonObject>

#include <CommonTools/Errors.h>
#include <CommonTools/GroupUuid.h>
#include <CommonTools/JsonTools.h>
#include <CommonTools/Validator.h>

class GroupJsonValidator : public Validator<QJsonObject>
{
public:
    static GroupJsonValidator defaultChecks(bool checkLastEdit = false)
    {
        GroupJsonValidator ret;
        ret.addCheck(&validUuidField);
        ret.addCheck(&validUuid);

        ret.addCheck(&validNameField);
        ret.addCheck(&nameNotEmpty);

        ret.addCheck(&validCommentField);

        ret.addCheck(&validSizeField);

        if (checkLastEdit) {
            ret.addCheck(&validLastEditField);
            ret.addCheck(&validLastEdit);
        }
        return ret;
    }

    static GroupJsonValidator fullChecks()
    {
        GroupJsonValidator ret;
        ret.addCheck(&validUuidField);
        ret.addCheck(&validUuid);

        ret.addCheck(&validNameField);
        ret.addCheck(&nameNotEmpty);

        ret.addCheck(&validCommentField);

        ret.addCheck(&validNodesArray);
        return ret;
    }

    static GroupJsonValidator staticDataChecks()
    {
        GroupJsonValidator ret;
        ret.addCheck(&validNameField);
        ret.addCheck(&nameNotEmpty);

        ret.addCheck(&validNodesArray);
        return ret;
    }

private:
    static CheckResult validUuidField(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::uuidKey].isString(), ErrorCodes::JsonUuidFieldMissedOrWrongType);
    }

    static CheckResult validUuid(const QJsonObject& obj)
    {
        return checkOrError(GroupUuid::from(obj[JsonTools::uuidKey].toString()).has_value(),
                            ErrorCodes::GroupUuidInvalid);
    }

    static CheckResult validNameField(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::nameKey].isString(), ErrorCodes::JsonNameFieldMissedOrWrongType);
    }

    static CheckResult nameNotEmpty(const QJsonObject& obj)
    {
        return checkOrError(!obj[JsonTools::nameKey].toString().isEmpty(), ErrorCodes::GroupNameEmpty);
    }

    static CheckResult validCommentField(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::commentKey].isString(), ErrorCodes::JsonCommentFieldMissedOrWrongType);
    }

    static CheckResult validSizeField(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::sizeKey].isDouble() && obj[JsonTools::sizeKey].toInt(-1) != -1,
                            ErrorCodes::JsonSizeFieldMissedOrWrongType);
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

    static CheckResult validNodesArray(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::termsKey].isArray(), ErrorCodes::JsonNodesFieldMissedOrWrongType);
    }

    static CheckResult validNodesLastEditField(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::nodesLastEditKey].isString(),
                            ErrorCodes::JsonNodesLastEditFieldMissedOrWrongType);
    }

    static CheckResult validNodesLastEdit(const QJsonObject& obj)
    {
        return checkOrError(!QDateTime::fromString(obj[JsonTools::nodesLastEditKey].toString(), Qt::ISODate).isNull(),
                            ErrorCodes::NodesLastEditInvalid);
    }
};
