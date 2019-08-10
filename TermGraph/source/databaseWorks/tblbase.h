#pragma once

#include <QtSql>

#include "source/Helpers/handytypes.h"
#include "source/databaseWorks/sqlqueryconstructor.h"

using SqCond = QMap<QString, QString>;
using RecVector = QVector<QSqlRecord>;

class TblBase
{
public:
    TblBase(QSqlDatabase *base);
    virtual ~TblBase() = default;

protected:
    QSqlQuery createTable();

    void setField(const TColumn& column, const QString &key, const QString &val);

    QString getStringField(const TColumn& column, const QString& key) const;

    QSqlQuery select(
            const TColumn& column,
            const WhereCondition &where = WhereCondition(),
            const QString &orderBy = "") const;

    QSqlQuery select(
            const TColumn::List &columns,
            const WhereCondition &where = WhereCondition(),
            const QString &orderBy = "") const;

    bool hasAnyRecord(const WhereCondition& where) const;

    bool insertInto(const InsertContainer::List& values);

    void updateWhere(const SetExpression &set, const WhereCondition &where);

    WhereCondition primaryKeyEqual(const QString &value) const;

    void deleteByKey(const QString &key);
    void deleteWhere(const WhereCondition& where);

    static RecVector toRecVector(QSqlQuery&& q);

    virtual const char* tableName() const = 0;
    virtual TColumn primaryKey() const = 0;
    virtual TColumn::List getAllColumns() const = 0;
    virtual void initTable() = 0;

    static UuidList filterEmptyUuids(const UuidList& uuids);

private:
    QSqlDatabase *base;
    bool isColumnExist(const TColumn& column) const;

    QSqlQuery startQuery(const QString& queryString) const;
    bool hasErrors(const QString &errString) const;

    QSqlQuery executeSelect(const QStringList &cols, const WhereCondition &where, const QString &orderBy = "") const;
    QSqlQuery executeInsert(const InsertContainer::List& values);
    void      executeUpdate(const SetExpression &set, const WhereCondition &where);
};
