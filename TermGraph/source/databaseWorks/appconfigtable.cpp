#include "appconfigtable.h"

void AppConfigTable::initTable()
{
    createTable();
    // Add database version parameter
    addValue(dbVersionPropertyName, dbVersion);

    getValue(dbVersionPropertyName, 10);
}

TColumn::List AppConfigTable::getAllColumns() const
{
    TColumn::List lst;
    for (auto col : AppConfigColumn::columns) {
        lst << col;
    }
    return lst;
}

void AppConfigTable::insertVersionFieldIfNeed()
{
    // Check for dbVersion record exist
    auto where = WhereCondition();
    where.equal(AppConfigColumn::parameter, dbVersionPropertyName);
    toRecList(select(AppConfigColumn::value, where));
}

void AppConfigTable::addValue(const QString &key, const int &value)
{
    addValue(key, QString::number(value));
}

void AppConfigTable::addValue(const QString &key, const QString &value)
{
    QList<InsertContainer> values;

    values << InsertContainer(AppConfigColumn::parameter, key);
    values << InsertContainer(AppConfigColumn::value, value);

    insertInto(values);
}

bool AppConfigTable::hasKey(const QString &key)
{
    auto where = WhereCondition();
    where.equal(AppConfigColumn::parameter, key);
    auto recList = toRecList(select(AppConfigColumn::parameter, where));
    return recList.size() == 1;
}

int AppConfigTable::getValue(const QString &key, const int &defaultValue)
{
    auto strValue = getValue(key, QString::number(defaultValue));
    bool* castOk = new bool(false);
    auto result = strValue.toInt(castOk);
    if (castOk) {
        delete castOk;
        return result;
    }
    delete castOk;
    return defaultValue;
}

QString AppConfigTable::getValue(const QString &key, const QString &defaultValue)
{
    if (hasKey(key)) {
        auto where = WhereCondition();
        where.equal(AppConfigColumn::parameter, key);
        auto recList = toRecList(select(AppConfigColumn::value, where));
        return recList.first().value(AppConfigColumn::value).toString();
    }

    addValue(key, defaultValue);
    return defaultValue;
}
