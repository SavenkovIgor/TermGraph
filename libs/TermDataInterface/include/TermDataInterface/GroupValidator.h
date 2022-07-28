// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QJsonObject>

#include <CommonTools/GroupUuid.h>
#include <CommonTools/Validator.h>


class GroupJsonValidator : public Validator<QJsonObject, ErrorCodes>
{
public:
    constexpr static auto uuidKey          = "uuid";
    constexpr static auto nameKey          = "name";
    constexpr static auto commentKey       = "comment";
    constexpr static auto sizeKey          = "size";
    constexpr static auto lastEditKey      = "lastEdit";

    constexpr static auto nodesKey         = "nodesList";
    constexpr static auto nodesLastEditKey = "nodesLastEdit";

    static GroupJsonValidator defaultChecks()
    {
        GroupJsonValidator ret;
        // clang-format off
        ret.addCheck(&validUuidField,     ErrorCodes::JsonUuidFieldMissedOrWrongType);
        ret.addCheck(&validUuid,          ErrorCodes::GroupUuidInvalid);

        ret.addCheck(&validNameField,     ErrorCodes::JsonNameFieldMissedOrWrongType);
        ret.addCheck(&nameNotEmpty,       ErrorCodes::GroupNameEmpty);

        ret.addCheck(&validCommentField,  ErrorCodes::JsonCommentFieldMissedOrWrongType);

        ret.addCheck(&validSizeField,     ErrorCodes::JsonSizeFieldMissedOrWrongType);

        ret.addCheck(&validLastEditField, ErrorCodes::JsonLastEditFieldMissedOrWrongType);
        ret.addCheck(&validLastEdit,      ErrorCodes::LastEditInvalid);
        // clang-format on
        return ret;
    }

    static GroupJsonValidator importChecks()
    {
        auto ret = defaultChecks();
        ret.addCheck(&validNodesArray,         ErrorCodes::JsonNodesFieldMissedOrWrongType);
        ret.addCheck(&validNodesLastEditField, ErrorCodes::JsonNodesLastEditFieldMissedOrWrongType);
        ret.addCheck(&validNodesLastEdit,      ErrorCodes::NodesLastEditInvalid);
        return ret;
    }

private:
    static bool validUuidField(const QJsonObject& obj) { return obj[uuidKey].isString(); }
    static bool validUuid(const QJsonObject& obj) { return GroupUuid::create(obj[uuidKey].toString()).has_value(); }

    static bool validNameField(const QJsonObject& obj) { return obj[nameKey].isString(); }
    static bool nameNotEmpty(const QJsonObject& obj) { return !(obj[nameKey].toString().isEmpty()); }

    static bool validCommentField(const QJsonObject& obj) { return obj[commentKey].isString(); }

    static bool validSizeField(const QJsonObject& obj) { return obj[sizeKey].isDouble() && obj[sizeKey].toInt(-1) != -1; }

    static bool validLastEditField(const QJsonObject& obj) { return obj[lastEditKey].isString(); }
    static bool validLastEdit(const QJsonObject& obj) { return !(QDateTime::fromString(obj[lastEditKey].toString(), Qt::ISODate).isNull()); }

    static bool validNodesArray(const QJsonObject& obj) { return obj[nodesKey].isArray(); }
    static bool validNodesLastEditField(const QJsonObject& obj) { return obj[nodesLastEditKey].isString(); }
    static bool validNodesLastEdit(const QJsonObject& obj) { return !(QDateTime::fromString(obj[nodesLastEditKey].toString(), Qt::ISODate).isNull()); }
};
