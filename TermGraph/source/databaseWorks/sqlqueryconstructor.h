#pragma once

#include <QtCore>

#include "source/databaseWorks/columns/tcolumn.h"
#include "source/databaseWorks/tools/insertcontainer.h"
#include "source/databaseWorks/tools/querytools.h"
#include "source/databaseWorks/tools/setexpression.h"
#include "source/databaseWorks/tools/wherecondition.h"

// Задача этого класса - формировать запросы
class SqlQueryConstructor
{
public:
    SqlQueryConstructor(QString tableName);

    QString selectQuery(const QStringList& columns,
                        const WhereCondition& where,
                        const QString& orderBy = "");
    QString selectOneQuery(const WhereCondition& where);
    QString insertQuery(const InsertContainer::List& values);
    QString updateQuery(const SetExpression& set, const WhereCondition& where);

    QString deleteWhereQuery(const WhereCondition& where);
    QString deleteByUuidQuery(const QUuid& uuid, const QString& primaryKeyName = "longUID");

    // Static methods
    static QString createTable(const QString& tableName, const TColumn::List& columns);
    static QString addColumn(const QString& tableName, const TColumn& column);

private:
    QString tableName;
};
