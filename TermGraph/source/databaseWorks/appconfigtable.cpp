#include "appconfigtable.h"

TColumn::List AppConfigTable::getAllColumns() const
{
    TColumn::List lst;
    for (auto col : NodeColumn::columns) {
        lst << col;
    }
    return lst;
}
