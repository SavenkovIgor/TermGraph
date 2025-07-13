// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QJsonObject>

#include "source/CommonTools/Validator.h"

class GroupJsonValidator : public Validator<QJsonObject>
{
public:
    static GroupJsonValidator defaultChecks(bool checkLastEdit = false);
    static GroupJsonValidator fullChecks();
    static GroupJsonValidator staticDataChecks();

private:
    static CheckResult validUuid(const QJsonObject& obj);
    static CheckResult validName(const QJsonObject& obj);
    static CheckResult validComment(const QJsonObject& obj);
    static CheckResult validSizeField(const QJsonObject& obj);
    static CheckResult validLastEdit(const QJsonObject& obj);
    static CheckResult validNodesArray(const QJsonObject& obj);
    static CheckResult validNodesLastEdit(const QJsonObject& obj);
};
