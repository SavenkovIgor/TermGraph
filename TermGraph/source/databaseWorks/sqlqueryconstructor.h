#pragma once

#include <QtCore>

#include "source/databaseWorks/columns/tcolumn.h"
#include "source/databaseWorks/tools/insertcontainer.h"
#include "source/databaseWorks/tools/querytools.h"
#include "source/databaseWorks/tools/setexpression.h"
#include "source/databaseWorks/tools/wherecondition.h"

// Задача этого класса - формировать запросы
// Clear static class
class SqlQueryConstructor
{
public:
    static QString createTable(const QString& tableName, const TColumn::List& columns);
    static QString addColumn(const QString& tableName, const TColumn& column);

    static QString selectQuery(const QString& tableName,
                               const QStringList& columns,
                               const WhereCondition& where,
                               const QString& orderBy = "");
    static QString selectOneQuery(const QString& tableName, const WhereCondition& where);
    static QString insertQuery(const QString& tableName, const InsertContainer::List& values);

    static QString updateQuery(const QString& tableName,
                               const SetExpression& set,
                               const WhereCondition& where);

    static QString deleteWhereQuery(const QString& tableName, const WhereCondition& where);
};
