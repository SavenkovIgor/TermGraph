#include "./sqlqueryconstructor.h"

SqlQueryConstructor::SqlQueryConstructor(QString tableName)
{
    this->tableName = tableName;
}

QString SqlQueryConstructor::createTable(QList<TableColumnDescription> columns)
{
    QStringList colsDescription;
    for (TableColumnDescription col : columns) {
        colsDescription << col.name + " " + col.type;
    }

    QStringList qry;
    qry << "CREATE";
    qry << "TABLE";
    qry << tableName;
    qry << "(";
    qry << colsDescription.join(CommonQueryFunctions::joinParam());
    qry << ")";

    return qry.join(" ");
}

QString SqlQueryConstructor::addColumn(TableColumnDescription column)
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

QString SqlQueryConstructor::selectQuery(QStringList columns, WhereCondition where, QString orderBy)
{
    QStringList qry;
    qry << "SELECT";
    qry << columns.join(CommonQueryFunctions::joinParam());
    qry << "FROM";
    qry << tableName;
    if (where.getJoinedConditions() != ""){
        qry << "WHERE";
        qry << where.getJoinedConditions();
    }
    if (orderBy.simplified() != "") {
       qry << "ORDER BY";
       qry << orderBy;
    }

    return qry.join(" ");
}

QString SqlQueryConstructor::insertQuery(QList<InsertContainer> values)
{
    QStringList columns;
    QStringList insertValues;

    for (InsertContainer value : values) {
        columns << value.getColumnName();
        insertValues << CommonQueryFunctions::vv(value.getValue());
    }

    QStringList qry;
    qry << "INSERT";
    qry << "INTO";
    qry << tableName;
    qry << "(";
    qry << columns.join(CommonQueryFunctions::joinParam());
    qry << ")";
    qry << "VALUES";
    qry << "(";
    qry << insertValues.join(CommonQueryFunctions::joinParam());
    qry << ")";

    return qry.join(" ");
}

QString SqlQueryConstructor::updateQuery(SetExpression set, WhereCondition where)
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

QString SqlQueryConstructor::deleteWhereQuery(WhereCondition where)
{
    QStringList qry;
    qry << "DELETE";
    qry << "FROM";
    qry << tableName;
    qry << "WHERE";
    qry << where.getJoinedConditions();

    return qry.join(" ");
}

QString SqlQueryConstructor::deleteByUuidQuery(QUuid uuid, QString primaryKeyName)
{
    QStringList qry;
    qry << "DELETE";
    qry << "FROM";
    qry << tableName;
    qry << "WHERE";
    qry << primaryKeyName;
    qry << "=";
    qry << CommonQueryFunctions::vv(uuid.toString());

    return qry.join(" ");
}



QStringList SqlQueryConstructor::vv(QStringList lst)
{
    for (QString& str : lst) {
        str = CommonQueryFunctions::vv(str);
    }
    return lst;
}
