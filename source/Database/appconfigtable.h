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

#include "source/Database/columns/tcolumn.h"
#include "source/Database/dbinfo.h"
#include "source/Database/tblbase.h"

class AppConfigColumn
{
public:
    constexpr static auto parameter = TColumn("parameter", "TEXT UNIQUE");
    constexpr static auto value     = TColumn("value", "TEXT");

    constexpr static TColumn columns[] = {parameter, value};
};

class AppConfigTable : public TblBase
{
public:
    AppConfigTable(QSqlDatabase* base)
        : TblBase(base)
    {}
    ~AppConfigTable() override = default;

    // This function must be called only once, when initing database
    void initTable() final;

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
