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

#include "termstorage/appconfigtable.h"

#include "source/dbtools.h"
#include "source/sqlquerybuilder.h"

void AppConfigTable::initTable()
{
    auto query = SqlQueryBuilder().createAppConfigTable();
    DbTools::startQuery2(query);
    // Add database version parameter
    setValue(dbVersionPropertyName, QString::number(dbVersion));
}

int AppConfigTable::getDbVersion() { return value(dbVersionPropertyName, "0").toInt(); }

bool AppConfigTable::isDbVersionActual() { return getDbVersion() == dbVersion; }

void AppConfigTable::updateDbVersionNumber() { setValue(dbVersionPropertyName, QString::number(dbVersion)); }

bool AppConfigTable::hasKey(const QString& key)
{
    auto query = SqlQueryBuilder().selectOneConfigParameter(key);
    DbTools::startQuery2(query);

    if (!query.next())
        return false;

    auto count = query.record().value("COUNT( * )").toInt();
    return count > 0;
}

void AppConfigTable::setValue(const QString& key, const QString& value)
{
    if (hasKey(key)) {
        // If has key - updating
        auto query = SqlQueryBuilder().updateConfigParameter(key, value);
        DbTools::startQuery2(query);
    } else {
        // Else adding new key
        auto query = SqlQueryBuilder().insertConfigParameter(key, value);
        DbTools::startQuery2(query);
    }
}

QString AppConfigTable::value(const QString& key, const QString& defaultValue)
{
    if (hasKey(key)) {
        auto query = SqlQueryBuilder().selectConfigParameter(key);
        DbTools::startQuery2(query);

        auto record = DbTools::getRecord(std::move(query));
        return record.value("value").toString();
    }

    return defaultValue;
}
