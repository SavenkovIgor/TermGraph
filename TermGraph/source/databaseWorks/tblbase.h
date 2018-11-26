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

    int getIntField(const QString& columnName, const QUuid& uuid);
    QString getStringField(const QString &columnName, const QUuid &uuid);

    void initColumn(const QString &columnName, const QString &columnType);
    QStringList getAllCols();

    QSqlQuery select(const QStringList &cols, const WhereCondition &where = WhereCondition(), const QString &orderBy = "");

    bool insertInto(const QList<InsertContainer> &values);

    void updateWhere(const SetExpression &set, const WhereCondition &where);

    void deleteRecord(const QUuid& uuid);
    void deleteWhere(const WhereCondition& where);

    RecList toRecList(QSqlQuery q);

private:
    QSqlDatabase *base;
    SqlQueryConstructor *queryConstructor;

    QString tableName;

    QList<TableColumnDescription> columns;
    bool isColumnNameExist(const QString& columnName) const;

    QSqlQuery startQuery(const QString& str);
    bool hasErrors(const QString &errString);

    QSqlQuery executeSelect(const QStringList &cols, const WhereCondition &where, const QString &orderBy = "");
    QSqlQuery executeInsert(const QList<InsertContainer> &values);
    void      executeUpdate(const SetExpression &set, const WhereCondition &where);

};

#endif // TBLBASE_H
