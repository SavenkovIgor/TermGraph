/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

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
