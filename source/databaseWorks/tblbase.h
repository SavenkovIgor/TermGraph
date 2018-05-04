#ifndef TBLBASE_H
#define TBLBASE_H

#include <QObject>
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
    void setField(QString columnName,int uid,QString val);
    void setField(QString columnName,QString uuid,QString val);

    int     getIntField   (QString columnName, int uid);
    QString getStringField(QString columnName, int uid);

    void initColumn(QString columnName, QString columnType);
    QStringList getAllCols();

    QSqlQuery select(QStringList cols, WhereConditions where = WhereConditions(), QString orderBy = "");

    bool insertInto(QList<InsertContainer> values);

    void updateWhere(SetExpression set, WhereConditions where);

    void deleteRecord(int id);
    void deleteWhere(WhereConditions where);

    RecList toRecList(QSqlQuery q);

private:
    QSqlDatabase *base;
    SqlQueryConstructor *queryConstructor;

    QString tableName;

    QList<TableColumnDescription> columns;
    bool isColumnNameExist(QString columnName);

    QSqlQuery startQuery(QString str);
    bool hasErrors(QString errString);

    QSqlQuery executeSelect(QStringList cols, WhereConditions where, QString orderBy = "");
    QSqlQuery executeInsert(QList<InsertContainer> values);
    void      executeUpdate(SetExpression set, WhereConditions where);

};

#endif // TBLBASE_H
