// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <QString>

export module AppConfigTable;

import DbTools;
import SqlQueryBuilder;

export class AppConfigTable
{
public:
    AppConfigTable()  = default;
    ~AppConfigTable() = default;

    // This function must be called only once, when initting database
    void initTable()
    {
        DbTools::start(SqlQueryBuilder().createAppConfigTable());
        // Add database version parameter
        setValue(dbVersionPropertyName, QString::number(dbVersion));
    }

    int getDbVersion() { return value(dbVersionPropertyName, "0").toInt(); }

    bool isDbVersionActual() { return getDbVersion() == dbVersion; }

    void updateDbVersionNumber() { setValue(dbVersionPropertyName, QString::number(dbVersion)); }

private:
    constexpr static auto dbVersionPropertyName = "dbVersion";
    constexpr static auto startDbVersion        = 1;
    constexpr static auto dbVersion             = 2;

    // Values works
    bool hasKey(const QString& key)
    {
        auto query = SqlQueryBuilder().selectOneConfigParameter(key);
        DbTools::start(query);

        if (!query.next()) {
            return false;
        }

        auto count = query.record().value("COUNT( * )").toInt();
        return count > 0;
    }

    void setValue(const QString& key, const QString& value)
    {
        if (hasKey(key)) {
            // If has key - updating
            DbTools::start(SqlQueryBuilder().updateConfigParameter(key, value));
        } else {
            // Else adding new key
            DbTools::start(SqlQueryBuilder().insertConfigParameter(key, value));
        }
    }

    QString value(const QString& key, const QString& defaultValue)
    {
        if (hasKey(key)) {
            auto query = SqlQueryBuilder().selectConfigParameter(key);
            DbTools::start(query);
            auto record = DbTools::getRecord(std::move(query));
            return record.value("value").toString();
        }

        return defaultValue;
    }
};
