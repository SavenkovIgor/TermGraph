#include "appconfigtable.h"

void AppConfigTable::initTable()
{
    createTable();

    QList<InsertContainer> values;
    values << InsertContainer(AppConfigColumn::parameter, dbVersionPropertyName);
    values << InsertContainer(AppConfigColumn::value, dbVersion);

    insertInto(values);
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
