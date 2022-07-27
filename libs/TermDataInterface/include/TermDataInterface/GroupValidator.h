// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QJsonObject>

#include <CommonTools/GroupUuid.h>
#include <CommonTools/Validator.h>

enum class GroupJsonErrors {
    UuidFieldMissedOrWrongType,
    InvalidUuid,

    NameFieldMissedOrWrongType,
    NameIsEmpty,

    CommentFieldMissedOrWrongType,

    SizeFieldMissedOrWrongType,

    LastEditFieldMissedOrWrongType,
    InvalidLastEdit,

    NodesFieldMissedOrWrongType,
    NodesLastEditFieldMissedOrWrongType,
    InvalidNodesLastEdit,
};

class GroupJsonValidator : public Validator<QJsonObject, GroupJsonErrors>
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
        ret.addCheck(&validUuidField,     GroupJsonErrors::UuidFieldMissedOrWrongType);
        ret.addCheck(&validUuid,          GroupJsonErrors::InvalidUuid);

        ret.addCheck(&validNameField,     GroupJsonErrors::NameFieldMissedOrWrongType);
        ret.addCheck(&nameNotEmpty,       GroupJsonErrors::NameIsEmpty);

        ret.addCheck(&validCommentField,  GroupJsonErrors::CommentFieldMissedOrWrongType);

        ret.addCheck(&validSizeField,     GroupJsonErrors::SizeFieldMissedOrWrongType);

        ret.addCheck(&validLastEditField, GroupJsonErrors::LastEditFieldMissedOrWrongType);
        ret.addCheck(&validLastEdit,      GroupJsonErrors::InvalidLastEdit);
        // clang-format on
        return ret;
    }

    static GroupJsonValidator importChecks()
    {
        auto ret = defaultChecks();
        ret.addCheck(&validNodesArray,         GroupJsonErrors::NodesFieldMissedOrWrongType);
        ret.addCheck(&validNodesLastEditField, GroupJsonErrors::NodesLastEditFieldMissedOrWrongType);
        ret.addCheck(&validNodesLastEdit,      GroupJsonErrors::InvalidNodesLastEdit);
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
