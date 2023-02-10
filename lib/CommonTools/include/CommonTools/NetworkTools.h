// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QtGlobal>

class NetworkTools
{
public:
    constexpr static qint16  defaultPort   = 26748;
    constexpr static quint16 localSyncPort = 46377;

    constexpr static auto termApiPath      = "/api/v1/global/terms";
    constexpr static auto termUuidApiPath  = "/api/v1/global/terms/:uuid";
    constexpr static auto groupApiPath     = "/api/v1/global/groups";
    constexpr static auto groupUuidApiPath = "/api/v1/global/groups/:uuid";
};
