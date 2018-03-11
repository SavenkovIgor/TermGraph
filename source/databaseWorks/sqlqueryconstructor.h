#ifndef SQLQUERYCONSTRUCTOR_H
#define SQLQUERYCONSTRUCTOR_H

#include <QtCore>

//FIXME: VV функция!!! не забудь!99
class SqlQueryConstructor
{
    static QString joinParam;

public:
    SqlQueryConstructor();

    static QString selectQuery(QString tableName, QString column,      QString where, QString addition);
    static QString selectQuery(QString tableName, QStringList columns, QString where, QString addition);

    static QString insertQuery(QString tableName, QString column,      QString value);
    static QString insertQuery(QString tableName, QStringList columns, QStringList values);

    static QString updateQuery(QString tableName, QString set, QString where);

    static QString deleteWhereQuery(QString tableName, QString where);
    static QString deleteByUidQuery(QString tableName, QString primaryKeyName, int id);
    static QString deleteByUidQuery(QString tableName, QString primaryKeyName, QString primaryKeyValue);

    static QString vv(QString str);
    static QStringList vv(QStringList lst);
};

#endif // SQLQUERYCONSTRUCTOR_H
