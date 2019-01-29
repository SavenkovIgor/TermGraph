#ifndef TBLBASE_H
#define TBLBASE_H

#include <QtSql>

#include "sqlqueryconstructor.h"

typedef QMap <QString, QString> SqCond;
typedef QList<QSqlRecord>       RecList;

class TblBase{
public:
    TblBase(QString tableName,QSqlDatabase *base);
    ~TblBase() = default;

    QSqlQuery createTable();
    void checkCols();

protected:
    void setField(const TColumn&column, const QUuid &uuid, const QString &val);
    void setField(const QString &columnName, const QUuid &uuid, const QString &val);

    int getIntField(const QString& columnName, const QUuid& uuid) const;
    QString getStringField(const QString &columnName, const QUuid &uuid) const;

    void initColumn(const TColumn& column);
    QStringList getAllCols() const;

    QSqlQuery select(
            const TColumn& column,
            const WhereCondition &where = WhereCondition(),
            const QString &orderBy = "") const;

    QSqlQuery select(
            const TColumn::List &columns,
            const WhereCondition &where = WhereCondition(),
            const QString &orderBy = "") const;

    QSqlQuery select(
            const QStringList &cols,
            const WhereCondition &where = WhereCondition(),
            const QString &orderBy = "") const;

    bool insertInto(const QList<InsertContainer> &values);

    void updateWhere(const SetExpression &set, const WhereCondition &where);

    void deleteRecord(const QUuid& uuid);
    void deleteWhere(const WhereCondition& where);

    static RecList toRecList(QSqlQuery q);

private:
    QSqlDatabase *base;
    QScopedPointer<SqlQueryConstructor> queryConstructor;

    QString tableName;

    TColumn::List columns;
    bool isColumnNameExist(const QString& columnName) const;

    QSqlQuery startQuery(const QString& str) const;
    bool hasErrors(const QString &errString) const;

    QSqlQuery executeSelect(const QStringList &cols, const WhereCondition &where, const QString &orderBy = "") const;
    QSqlQuery executeInsert(const QList<InsertContainer> &values);
    void      executeUpdate(const SetExpression &set, const WhereCondition &where);

};

#endif // TBLBASE_H
