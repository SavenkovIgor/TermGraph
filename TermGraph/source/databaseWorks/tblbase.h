#ifndef TBLBASE_H
#define TBLBASE_H

#include <QtSql>

#include "sqlqueryconstructor.h"

typedef QMap <QString, QString> SqCond;
typedef QList<QSqlRecord>       RecList;

class TblBase{
public:
    TblBase(QString tableName,QSqlDatabase *base);

    QSqlQuery createTable();
    void checkCols();

protected:
    void setField(const QString &columnName, const QUuid &uuid, const QString &val);

    int getIntField(QString columnName, QUuid uuid);
    QString getStringField(QString columnName, QUuid uuid);

    void initColumn(QString columnName, QString columnType);
    QStringList getAllCols();

    QSqlQuery select(QStringList cols, WhereCondition where = WhereCondition(), QString orderBy = "");

    bool insertInto(QList<InsertContainer> values);

    void updateWhere(const SetExpression &set, const WhereCondition &where);

    void deleteRecord(QUuid uuid);
    void deleteWhere(WhereCondition where);

    RecList toRecList(QSqlQuery q);

private:
    QSqlDatabase *base;
    SqlQueryConstructor *queryConstructor;

    QString tableName;

    QList<TableColumnDescription> columns;
    bool isColumnNameExist(QString columnName);

    QSqlQuery startQuery(const QString& str);
    bool hasErrors(QString errString);

    QSqlQuery executeSelect(QStringList cols, WhereCondition where, QString orderBy = "");
    QSqlQuery executeInsert(QList<InsertContainer> values);
    void      executeUpdate(const SetExpression &set, const WhereCondition &where);

};

#endif // TBLBASE_H
