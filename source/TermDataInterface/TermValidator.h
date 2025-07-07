// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QJsonObject>

#include "source/CommonTools/Validator.h"

class TermJsonValidator : public Validator<QJsonObject>
{
public:
    TermJsonValidator(bool checkUuid = true, bool checkLastEdit = true);

    static TermJsonValidator minimalStaticDataChecks();

private:
    static CheckResult validUuid(const QJsonObject& obj);
    static CheckResult validTerm(const QJsonObject& obj);
    static CheckResult validDefinition(const QJsonObject& obj);
    static CheckResult validTermDef(const QJsonObject& obj);
    static CheckResult validDescription(const QJsonObject& obj);
    static CheckResult validExamples(const QJsonObject& obj);
    static CheckResult validWikiUrl(const QJsonObject& obj);
    static CheckResult validWikiImage(const QJsonObject& obj);
    static CheckResult validGroupUuid(const QJsonObject& obj);
    static CheckResult validLastEditField(const QJsonObject& obj);
    static CheckResult validLastEdit(const QJsonObject& obj);
};
