// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <QLatin1String>

export module DbInfo;

export class DbConnectionName
{
public:
    constexpr static auto defaultConnection       = "defConnection";
    constexpr static auto threadLoadingConnection = "loadThreadConnection";
};
