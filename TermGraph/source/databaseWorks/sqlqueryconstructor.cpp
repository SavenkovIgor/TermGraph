#include "source/databaseWorks/sqlqueryconstructor.h"
#include "source/databaseWorks/tools/querytools.h"

QString SqlQueryConstructor::createTable(const QString& tableName, const TColumn::List& columns)
{
    QStringList colsDescription;
    for (auto col : columns)
        colsDescription << QString(col.name) + " " + QString(col.type);

    QStringList qry;

    qry << "CREATE TABLE";
    qry << tableName;
    qry << "(" + colsDescription.join(QueryTools::joinParam) + ")";

    return qry.join(" ");
}

QString SqlQueryConstructor::addColumn(const QString& tableName, const TColumn& column)
{
    QStringList qry;
    qry << "ALTER TABLE";
    qry << tableName;
    qry << "ADD COLUMN";
    qry << column.name;
    qry << column.type;

    return qry.join(" ");
}

QString SqlQueryConstructor::dropTable(const QString& tableName)
{
    return "DROP TABLE " + tableName;
}

QString SqlQueryConstructor::recordsCount(const QString &tableName)
{
    return "SELECT COUNT(*) FROM " + tableName;
}

QString SqlQueryConstructor::selectQuery(const QString& tableName,
                                         const QStringList& columns,
                                         const WhereCondition& where,
                                         const QString& orderBy)
{
    QStringList qry;
    qry << "SELECT";
    qry << columns.join(QueryTools::joinParam);
    qry << "FROM";
    qry << tableName;
    if (where.getJoinedConditions() != "") {
        qry << "WHERE";
        qry << where.getJoinedConditions();
    }
    if (orderBy.simplified() != "") {
        qry << "ORDER BY";
        qry << orderBy;
    }

    return qry.join(" ");
}

QString SqlQueryConstructor::selectOneQuery(const QString& tableName, const WhereCondition& where)
{
    QStringList qry;
    qry << "SELECT COUNT(*) FROM";
    qry << tableName;
    if (where.getJoinedConditions() != "") {
        qry << "WHERE";
        qry << where.getJoinedConditions();
    }

    qry << "LIMIT 1";

    return qry.join(" ");
}

QString SqlQueryConstructor::insertQuery(const QString& tableName,
                                         const InsertContainer::List& values)
{
    QStringList columns;
    QStringList insertValues;

    for (const auto& value : values) {
        columns << value.getColumnName();
        insertValues << QueryTools::vv(value.getValue());
    }

    QStringList qry;
    qry << "INSERT INTO";
    qry << tableName;
    qry << "( " + columns.join(QueryTools::joinParam) + " )";
    qry << "VALUES";
    qry << "( " + insertValues.join(QueryTools::joinParam) + " )";

    return qry.join(" ");
}

QString SqlQueryConstructor::updateQuery(const QString& tableName,
                                         const SetExpression& set,
                                         const WhereCondition& where)
{
    QStringList qry;
    qry << "UPDATE";
    qry << tableName;
    qry << "SET";
    qry << set.getExpression();
    qry << "WHERE";
    qry << where.getJoinedConditions();

    return qry.join(" ");
}

QString SqlQueryConstructor::deleteWhereQuery(const QString& tableName, const WhereCondition& where)
{
    QStringList qry;
    qry << "DELETE FROM";
    qry << tableName;
    qry << "WHERE";
    qry << where.getJoinedConditions();

    return qry.join(" ");
}
