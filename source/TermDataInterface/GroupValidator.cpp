// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <expected>

#include <QDateTime>
#include <QJsonObject>

#include "source/CommonTools/Errors.h"
#include "source/CommonTools/GroupUuid.h"

export module TermDataInterface.GroupValidator;

import CommonTools.Validator;
import CommonTools.JsonTools;

using namespace std;

export class GroupJsonValidator : public Validator<QJsonObject>
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
            return unexpected(ErrorCode::JsonUuidFieldMissedOrWrongType);

        if (auto uuid = GroupUuid::from(field.toString()); !uuid.has_value())
            return unexpected(ErrorCode::GroupUuidInvalid);

        return {};
    }

    static CheckResult validName(const QJsonObject& obj)
    {
        const auto& field = obj[JsonTools::nameKey];

        if (!field.isString())
            return unexpected(ErrorCode::JsonNameFieldMissedOrWrongType);

        if (field.toString().isEmpty())
            return unexpected(ErrorCode::GroupNameEmpty);

        return {};
    }

    static CheckResult validComment(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::commentKey].isString(), ErrorCode::JsonCommentFieldMissedOrWrongType);
    }

    static CheckResult validSizeField(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::sizeKey].isDouble() && obj[JsonTools::sizeKey].toInt(-1) != -1,
                            ErrorCode::JsonSizeFieldMissedOrWrongType);
    }

    static CheckResult validLastEdit(const QJsonObject& obj)
    {
        const auto& field = obj[JsonTools::lastEditKey];

        if (!field.isString())
            return unexpected(ErrorCode::JsonLastEditFieldMissedOrWrongType);

        if (auto dt = QDateTime::fromString(field.toString(), Qt::ISODate); dt.isNull())
            return unexpected(ErrorCode::LastEditInvalid);

        return {};
    }

    static CheckResult validNodesArray(const QJsonObject& obj)
    {
        return checkOrError(obj[JsonTools::termsKey].isArray(), ErrorCode::JsonNodesFieldMissedOrWrongType);
    }

    static CheckResult validNodesLastEdit(const QJsonObject& obj)
    {
        const auto& field = obj[JsonTools::nodesLastEditKey];

        if (!field.isString())
            return unexpected(ErrorCode::JsonNodesLastEditFieldMissedOrWrongType);

        if (auto dt = QDateTime::fromString(field.toString(), Qt::ISODate); dt.isNull())
            return unexpected(ErrorCode::NodesLastEditInvalid);

        return {};
    }
};
