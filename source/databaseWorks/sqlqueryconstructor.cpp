#include "sqlqueryconstructor.h"

QString SqlQueryConstructor::joinParam = ", ";

SqlQueryConstructor::SqlQueryConstructor()
{

}

QString SqlQueryConstructor::selectQuery(QString tableName, QString column, QString where, QString addition)
{
    return selectQuery(tableName,QStringList()<<column,where,addition);
}

QString SqlQueryConstructor::selectQuery(QString tableName, QStringList columns, QString where, QString addition)
{
    QStringList qry;
    qry << "SELECT";
    qry << columns.join(joinParam);
    qry << "FROM";
    qry << tableName;
    if( where.simplified() != "" ){
        qry << "WHERE";
        qry << where;
    }
    qry<<addition;

    return qry.join(" ");
}

QString SqlQueryConstructor::insertQuery(QString tableName, QString column, QString value)
{
    return insertQuery(tableName,QStringList()<<column,QStringList()<<value);
}

QString SqlQueryConstructor::insertQuery(QString tableName, QStringList columns, QStringList values)
{
    if (columns.size() != values.size())
        return "";

    QStringList qry;
    qry << "INSERT INTO";
    qry << tableName;
    qry << "(";
    qry << columns.join(joinParam);
    qry << ") VALUES (";
    qry << values.join(joinParam);
    qry << ")";

    return qry.join(" ");
}

QString SqlQueryConstructor::updateQuery(QString tableName, QString set, QString where)
{
    QStringList qry;
    qry << "UPDATE";
    qry << tableName;
    qry << "SET";
    qry << set;
    qry << "WHERE";
    qry << where;

    return qry.join(" ");
}

QString SqlQueryConstructor::deleteWhereQuery(QString tableName, QString where)
{
    QStringList qry;
    qry << "DELETE";
    qry << "FROM";
    qry << tableName;
    qry << "WHERE";
    qry << where;

    return qry.join(" ");
}

QString SqlQueryConstructor::deleteByUidQuery(QString tableName, QString primaryKeyName, int id)
{
    return deleteByUidQuery(tableName,primaryKeyName,QString::number(id));
}

QString SqlQueryConstructor::deleteByUidQuery(QString tableName, QString primaryKeyName, QString primaryKeyValue)
{
    QStringList qry;
    qry << "DELETE";
    qry << "FROM";
    qry << tableName;
    qry << "WHERE";
    qry << primaryKeyName;
    qry << "=";
    qry << vv(primaryKeyValue);

    return qry.join(" ");
}

QString SqlQueryConstructor::vv(QString str)
{
    str.replace("'","''");
    str.replace("\"","\"\"");
    QString ret = "'" + str + "'";
    return ret;
}

QStringList SqlQueryConstructor::vv(QStringList lst)
{
    for(QString &str: lst) {
        str = vv(str);
    }
    return lst;
}
