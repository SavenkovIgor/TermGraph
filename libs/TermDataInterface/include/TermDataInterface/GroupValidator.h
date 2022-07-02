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
    CommentNotString,

    NodesFieldMissedOrWrongType
};

class GroupJsonValidator : public Validator<QJsonObject, GroupJsonErrors>
{
public:
    constexpr static auto uuidKey    = "uuid";
    constexpr static auto nameKey    = "name";
    constexpr static auto commentKey = "comment";
    constexpr static auto nodesKey   = "nodesList";

    static GroupJsonValidator defaultChecks()
    {
        GroupJsonValidator ret;
        ret.addCheck(&validUuidField, GroupJsonErrors::UuidFieldMissedOrWrongType);
        ret.addCheck(&validUuid, GroupJsonErrors::InvalidUuid);

        ret.addCheck(&validNameField, GroupJsonErrors::NameFieldMissedOrWrongType);
        ret.addCheck(&nameNotEmpty, GroupJsonErrors::NameIsEmpty);

        ret.addCheck(&validCommentField, GroupJsonErrors::CommentFieldMissedOrWrongType);
        return ret;
    }

    static GroupJsonValidator importChecks()
    {
        auto ret = defaultChecks();
        ret.addCheck(&validNodesArray, GroupJsonErrors::NodesFieldMissedOrWrongType);
        return ret;
    }

private:
    static bool validUuidField(const QJsonObject& obj) { return obj[uuidKey].isString(); }
    static bool validUuid(const QJsonObject& obj) { return GroupUuid::create(obj[uuidKey].toString()).has_value(); }

    static bool validNameField(const QJsonObject& obj) { return obj[nameKey].isString(); }
    static bool nameNotEmpty(const QJsonObject& obj) { return !(obj[nameKey].toString().isEmpty()); }

    static bool validCommentField(const QJsonObject& obj) { return obj[commentKey].isString(); }

    static bool validNodesArray(const QJsonObject& obj) { return obj[nodesKey].isArray(); }
};
