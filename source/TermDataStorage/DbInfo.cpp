// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <QString>
#include <QLatin1String>

export module DbInfo;

export class DbConnection
{
public:
    constexpr static auto defaultConnection       = "defConnection";
    constexpr static auto threadLoadingConnection = "loadThreadConnection";

    static QString mDbFilePath;
};

QString DbConnection::mDbFilePath     = "";
