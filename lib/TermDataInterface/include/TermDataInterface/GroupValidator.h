// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QJsonObject>

#include <CommonTools/GroupUuid.h>
#include <CommonTools/JsonTools.h>
#include <CommonTools/Validator.h>

class GroupJsonValidator : public Validator<QJsonObject, ErrorCodes>
{
public:
    static GroupJsonValidator defaultChecks(bool checkLastEdit = false)
    {
        GroupJsonValidator ret;
        // clang-format off
        ret.addCheck(&validUuidField,     ErrorCodes::JsonUuidFieldMissedOrWrongType);
        ret.addCheck(&validUuid,          ErrorCodes::GroupUuidInvalid);

        ret.addCheck(&validNameField,     ErrorCodes::JsonNameFieldMissedOrWrongType);
        ret.addCheck(&nameNotEmpty,       ErrorCodes::GroupNameEmpty);

        ret.addCheck(&validCommentField,  ErrorCodes::JsonCommentFieldMissedOrWrongType);

        ret.addCheck(&validSizeField,     ErrorCodes::JsonSizeFieldMissedOrWrongType);

        if (checkLastEdit) {
            ret.addCheck(&validLastEditField, ErrorCodes::JsonLastEditFieldMissedOrWrongType);
            ret.addCheck(&validLastEdit,      ErrorCodes::LastEditInvalid);
        }
        // clang-format on
        return ret;
    }

    static GroupJsonValidator fullChecks()
    {
        GroupJsonValidator ret;
        ret.addCheck(&validUuidField, ErrorCodes::JsonUuidFieldMissedOrWrongType);
        ret.addCheck(&validUuid, ErrorCodes::GroupUuidInvalid);

        ret.addCheck(&validNameField, ErrorCodes::JsonNameFieldMissedOrWrongType);
        ret.addCheck(&nameNotEmpty, ErrorCodes::GroupNameEmpty);

        ret.addCheck(&validCommentField, ErrorCodes::JsonCommentFieldMissedOrWrongType);

        ret.addCheck(&validNodesArray, ErrorCodes::JsonNodesFieldMissedOrWrongType);
        return ret;
    }

    static GroupJsonValidator staticDataChecks()
    {
        GroupJsonValidator ret;
        ret.addCheck(&validUuidField, ErrorCodes::JsonUuidFieldMissedOrWrongType);
        ret.addCheck(&validUuid, ErrorCodes::GroupUuidInvalid);

        ret.addCheck(&validNameField, ErrorCodes::JsonNameFieldMissedOrWrongType);
        ret.addCheck(&nameNotEmpty, ErrorCodes::GroupNameEmpty);

        ret.addCheck(&validNodesArray, ErrorCodes::JsonNodesFieldMissedOrWrongType);
        return ret;
    }

private:
    static bool validUuidField(const QJsonObject& obj) { return obj[JsonTools::uuidKey].isString(); }
    static bool validUuid(const QJsonObject& obj)
    {
        return GroupUuid::from(obj[JsonTools::uuidKey].toString()).has_value();
    }

    static bool validNameField(const QJsonObject& obj) { return obj[JsonTools::nameKey].isString(); }
    static bool nameNotEmpty(const QJsonObject& obj) { return !(obj[JsonTools::nameKey].toString().isEmpty()); }

    static bool validCommentField(const QJsonObject& obj) { return obj[JsonTools::commentKey].isString(); }

    static bool validSizeField(const QJsonObject& obj)
    {
        return obj[JsonTools::sizeKey].isDouble() && obj[JsonTools::sizeKey].toInt(-1) != -1;
    }

    static bool validLastEditField(const QJsonObject& obj) { return obj[JsonTools::lastEditKey].isString(); }
    static bool validLastEdit(const QJsonObject& obj)
    {
        return !(QDateTime::fromString(obj[JsonTools::lastEditKey].toString(), Qt::ISODate).isNull());
    }

    static bool validNodesArray(const QJsonObject& obj) { return obj[JsonTools::termsKey].isArray(); }
    static bool validNodesLastEditField(const QJsonObject& obj) { return obj[JsonTools::nodesLastEditKey].isString(); }
    static bool validNodesLastEdit(const QJsonObject& obj)
    {
        return !(QDateTime::fromString(obj[JsonTools::nodesLastEditKey].toString(), Qt::ISODate).isNull());
    }
};
