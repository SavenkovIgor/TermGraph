// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QJsonDocument>
#include <QJsonObject>

#include <CommonTools/HandyTypes.h>

class JsonTools
{
public:
    static inline QString prepareUuidParameter(QString param)
    {
        param.remove("%7B");
        param.remove("%7D");
        param.remove('{');
        param.remove('}');

        param.prepend('{');
        param.append('}');

        return param;
    }

    constexpr static auto groupsKey     = "groups";
    constexpr static auto groupUuidsKey = "groupUuids";

    constexpr static auto termsKey     = "terms";
    constexpr static auto termUuidsKey = "termUuids";
};
