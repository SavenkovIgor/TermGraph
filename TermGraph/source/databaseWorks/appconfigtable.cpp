#include "appconfigtable.h"

void AppConfigTable::initTable()
{
    createTable();
    // Add database version parameter
    setValue(dbVersionPropertyName, QString::number(startDbVersion));
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
    auto recVector = toRecVector(select(AppConfigColumn::parameter, where));
    return recVector.size() == 1;
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
        QList<InsertContainer> values;

        values << InsertContainer(AppConfigColumn::parameter, key);
        values << InsertContainer(AppConfigColumn::value, value);

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
