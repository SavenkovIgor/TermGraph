#pragma once

#include <vector>

#include "source/databaseWorks/columns/tcolumn.h"

class InsertContainer
{
public:
    using List = std::vector<InsertContainer>;

    InsertContainer(const TColumn& column, const QString& value);
    InsertContainer(const TColumn& column, const int& value);

    QString getColumnName() const;
    QString getValue() const;

private:
    QString columnName;
    QString value;
};
