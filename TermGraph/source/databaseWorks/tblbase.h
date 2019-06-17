#ifndef TBLBASE_H
#define TBLBASE_H

#include <QtSql>

#include "../Helpers/handytypes.h"
#include "./sqlqueryconstructor.h"

using SqCond = QMap<QString, QString>;
using RecVector = QVector<QSqlRecord>;

class TblBase
{
public:
    TblBase(QString tableName, QSqlDatabase *base);
    virtual ~TblBase() = default;

protected:
    QSqlQuery createTable();
    void checkCols();

    void setField(const TColumn& column, const QUuid &uuid, const QString &val);

    int getIntField(const TColumn& column, const QUuid& uuid) const;
    QString getStringField(const TColumn& column, const QUuid &uuid) const;

    QSqlQuery select(
            const TColumn& column,
            const WhereCondition &where = WhereCondition(),
            const QString &orderBy = "") const;

    QSqlQuery select(
            const TColumn::List &columns,
            const WhereCondition &where = WhereCondition(),
            const QString &orderBy = "") const;

    bool hasAnyRecord(const WhereCondition& where) const;

    bool insertInto(const QList<InsertContainer> &values);

    void updateWhere(const SetExpression &set, const WhereCondition &where);

    void deleteRecord(const QUuid& uuid);
    void deleteWhere(const WhereCondition& where);

    static RecVector toRecVector(QSqlQuery&& q);

    virtual TColumn::List getAllColumns() const = 0;
    virtual void initTable() = 0;

    static UuidList filterEmptyUuids(const UuidList& uuids);

private:
    QSqlDatabase *base;
    QScopedPointer<SqlQueryConstructor> queryConstructor;

    bool isColumnExist(const TColumn& column) const;

    QSqlQuery startQuery(const QString& str) const;
    bool hasErrors(const QString &errString) const;

    QSqlQuery executeSelect(const QStringList &cols, const WhereCondition &where, const QString &orderBy = "") const;
    QSqlQuery executeInsert(const QList<InsertContainer> &values);
    void      executeUpdate(const SetExpression &set, const WhereCondition &where);
};

#endif // TBLBASE_H
