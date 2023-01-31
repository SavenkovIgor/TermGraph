// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/tables/AppConfigTable.h"

#include "source/DbTools.h"
#include "source/SqlQueryBuilder.h"

void AppConfigTable::initTable()
{
    DbTools::start(SqlQueryBuilder().createAppConfigTable());
    // Add database version parameter
    setValue(dbVersionPropertyName, QString::number(dbVersion));
}

int AppConfigTable::getDbVersion() { return value(dbVersionPropertyName, "0").toInt(); }

bool AppConfigTable::isDbVersionActual() { return getDbVersion() == dbVersion; }

void AppConfigTable::updateDbVersionNumber() { setValue(dbVersionPropertyName, QString::number(dbVersion)); }

bool AppConfigTable::hasKey(const QString& key)
{
    auto query = SqlQueryBuilder().selectOneConfigParameter(key);
    DbTools::start(query);

    if (!query.next()) {
        return false;
    }

    auto count = query.record().value("COUNT( * )").toInt();
    return count > 0;
}

void AppConfigTable::setValue(const QString& key, const QString& value)
{
    if (hasKey(key)) {
        // If has key - updating
        DbTools::start(SqlQueryBuilder().updateConfigParameter(key, value));
    } else {
        // Else adding new key
        DbTools::start(SqlQueryBuilder().insertConfigParameter(key, value));
    }
}

QString AppConfigTable::value(const QString& key, const QString& defaultValue)
{
    if (hasKey(key)) {
        auto query = SqlQueryBuilder().selectConfigParameter(key);
        DbTools::start(query);
        auto record = DbTools::getRecord(std::move(query));
        return record.value("value").toString();
    }

    return defaultValue;
}
