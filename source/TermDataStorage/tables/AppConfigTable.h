// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QString>

class AppConfigTable
{
public:
    AppConfigTable()  = default;
    ~AppConfigTable() = default;

    // This function must be called only once, when initting database
    void initTable();

    int  getDbVersion();
    bool isDbVersionActual();
    void updateDbVersionNumber();

private:
    constexpr static auto dbVersionPropertyName = "dbVersion";
    constexpr static auto startDbVersion        = 1;
    constexpr static auto dbVersion             = 2;

    // Values works
    bool hasKey(const QString& key);

    void    setValue(const QString& key, const QString& value);
    QString value(const QString& key, const QString& defaultValue);
};
