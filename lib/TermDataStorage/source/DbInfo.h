// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QLatin1String>

class DbConnectionName
{
public:
    constexpr static auto defaultConnection       = "defConnection";
    constexpr static auto threadLoadingConnection = "loadThreadConnection";
};
