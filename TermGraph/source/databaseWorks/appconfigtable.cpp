#include "appconfigtable.h"

void AppConfigTable::initTable()
{
    createTable();
    // TODO: Fill fields!
}

TColumn::List AppConfigTable::getAllColumns() const
{
    TColumn::List lst;
    for (auto col : AppConfigColumn::columns) {
        lst << col;
    }
    return lst;
}
