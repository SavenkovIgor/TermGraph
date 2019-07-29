#include "insertcontainer.h"

InsertContainer::InsertContainer(const TColumn& column, const QString& value)
    : columnName(column.name)
    , value(value)
{}

InsertContainer::InsertContainer(const TColumn& column, const int& value)
    : columnName(column.name)
    , value(QString::number(value))
{}

QString InsertContainer::getColumnName() const
{
    return columnName;
}

QString InsertContainer::getValue() const
{
    return value;
}
