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
    void setField(QString columnName, int uid, QString val); //TODO: Удалить когда все данные будут сохранены
    void setField(QString columnName, QUuid uuid, QString val);

    int     getIntField   (QString columnName, int uid);
    QString getStringField(QString columnName, int uid);

    void initColumn(QString columnName, QString columnType);
    QStringList getAllCols();

    QSqlQuery select(QStringList cols, WhereCondition where = WhereCondition(), QString orderBy = "");

    bool insertInto(QList<InsertContainer> values);

    void updateWhere(SetExpression set, WhereCondition where);

    void deleteRecord(QUuid uuid);
    void deleteWhere(WhereCondition where);

    RecList toRecList(QSqlQuery q);

private:
    QSqlDatabase *base;
    SqlQueryConstructor *queryConstructor;

    QString tableName;

    QList<TableColumnDescription> columns;
    bool isColumnNameExist(QString columnName);

    QSqlQuery startQuery(QString str);
    bool hasErrors(QString errString);

    QSqlQuery executeSelect(QStringList cols, WhereCondition where, QString orderBy = "");
    QSqlQuery executeInsert(QList<InsertContainer> values);
    void      executeUpdate(SetExpression set, WhereCondition where);

};

#endif // TBLBASE_H
