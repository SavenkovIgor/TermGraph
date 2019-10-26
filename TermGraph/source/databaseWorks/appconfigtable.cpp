/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
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

#include "appconfigtable.h"

void AppConfigTable::initTable()
{
    createTable();
    // Add database version parameter
    setValue(dbVersionPropertyName, QString::number(dbVersion));
}

int AppConfigTable::getDbVersion()
{
    return value(dbVersionPropertyName, "0").toInt();
}

bool AppConfigTable::isDbVersionActual()
{
    return getDbVersion() == dbVersion;
}

void AppConfigTable::updateDbVersionNumber()
{
    setValue(dbVersionPropertyName, QString::number(dbVersion));
}

const char* AppConfigTable::tableName() const
{
    return TableName::CONFIG;
}

TColumn AppConfigTable::primaryKey() const
{
    return AppConfigColumn::parameter;
}

TColumn::List AppConfigTable::getAllColumns() const
{
    TColumn::List lst;
    for (auto col : AppConfigColumn::columns) {
        lst << col;
    }
    return lst;
}

bool AppConfigTable::hasKey(const QString &key)
{
    WhereCondition where;
    where.equal(AppConfigColumn::parameter, key);
    return hasAnyRecord(where);
}

void AppConfigTable::setValue(const QString &key, const QString &value)
{
    if (hasKey(key)) {
        // If has key - updating
        SetExpression set;
        set.set(AppConfigColumn::value, value);

        WhereCondition where;
        where.equal(AppConfigColumn::parameter, key);

        updateWhere(set, where);
    } else {
        // Else adding new key
        InsertContainer::List values;

        values.push_back(InsertContainer(AppConfigColumn::parameter, key));
        values.push_back(InsertContainer(AppConfigColumn::value, value));

        insertInto(values);
    }
}

QString AppConfigTable::value(const QString &key, const QString &defaultValue)
{
    if (hasKey(key)) {
        auto where = WhereCondition();
        where.equal(AppConfigColumn::parameter, key);
        auto recList = toRecVector(select(AppConfigColumn::value, where));
        return recList.first().value(AppConfigColumn::value).toString();
    }

    return defaultValue;
}
