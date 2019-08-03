#include "source/databaseWorks/sqlqueryconstructor.h"
#include "source/databaseWorks/tools/querytools.h"

SqlQueryConstructor::SqlQueryConstructor(QString tableName)
{
    this->tableName = tableName;
}

QString SqlQueryConstructor::createTable(const TColumn::List& columns)
{
    return createTable(tableName, columns);
}

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

QString SqlQueryConstructor::addColumn(TColumn column)
{
    QStringList qry;
    qry << "ALTER";
    qry << "TABLE";
    qry << tableName;
    qry << "ADD COLUMN";
    qry << column.name;
    qry << column.type;

    return qry.join(" ");
}

QString SqlQueryConstructor::selectQuery(const QStringList& columns, const WhereCondition& where, const QString& orderBy)
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

QString SqlQueryConstructor::selectOneQuery(const WhereCondition &where)
{
    QStringList qry;
    qry << "SELECT";
    qry << "COUNT(*)";
    qry << "FROM";
    qry << tableName;
    if (where.getJoinedConditions() != "") {
        qry << "WHERE";
        qry << where.getJoinedConditions();
    }

    qry << "LIMIT 1";

    return qry.join(" ");
}

QString SqlQueryConstructor::insertQuery(const InsertContainer::List& values)
{
    QStringList columns;
    QStringList insertValues;

    for (const auto& value : values) {
        columns << value.getColumnName();
        insertValues << QueryTools::vv(value.getValue());
    }

    QStringList qry;
    qry << "INSERT";
    qry << "INTO";
    qry << tableName;
    qry << "(";
    qry << columns.join(QueryTools::joinParam);
    qry << ")";
    qry << "VALUES";
    qry << "(";
    qry << insertValues.join(QueryTools::joinParam);
    qry << ")";

    return qry.join(" ");
}

QString SqlQueryConstructor::updateQuery(const SetExpression& set, const WhereCondition& where)
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

QString SqlQueryConstructor::deleteWhereQuery(const WhereCondition& where)
{
    QStringList qry;
    qry << "DELETE";
    qry << "FROM";
    qry << tableName;
    qry << "WHERE";
    qry << where.getJoinedConditions();

    return qry.join(" ");
}

QString SqlQueryConstructor::deleteByUuidQuery(const QUuid& uuid, const QString& primaryKeyName)
{
    QStringList qry;
    qry << "DELETE";
    qry << "FROM";
    qry << tableName;
    qry << "WHERE";
    qry << primaryKeyName;
    qry << "=";
    qry << QueryTools::vv(uuid.toString());

    return qry.join(" ");
}
