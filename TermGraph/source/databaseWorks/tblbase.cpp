#include "tblbase.h"

#include "source/databaseWorks/tools/dbtools.h"

int TblBase::getIntField(const TColumn &column, const QUuid &uuid) const
{
    if (!isColumnExist(column)) {
        return -1;
    }

    QSqlQuery sel = select(column, WhereCondition::uuidEqual(uuid));

    if (!sel.next()) {
        return -1;
    }

    return sel.record().value(column).toInt();
}

QString TblBase::getStringField(const TColumn& column, const QUuid& uuid) const
{
    Q_ASSERT(isColumnExist(column));

    QSqlQuery sel = select(column, WhereCondition::uuidEqual(uuid));

    if (!sel.next()) {
        return "";
    }

    return sel.record().value(column).toString();
}

QSqlQuery TblBase::startQuery(const QString& queryString) const
{
    return DbTools::startQuery(base, queryString);
}

bool TblBase::hasErrors(const QString& errString) const
{
    bool ret;
    ret = (errString != "" && errString != " ");
    return ret;
}

TblBase::TblBase(QSqlDatabase* base)
    : base(base)
{}

QSqlQuery TblBase::createTable()
{
    auto query = SqlQueryConstructor::createTable(tableName(), getAllColumns());
    return startQuery(query);
}

bool TblBase::insertInto(const InsertContainer::List& values)
{
    QSqlQuery q = executeInsert(values);

    if (hasErrors(q.lastError().text())) {
        return false;
    }

    return true;
}

QSqlQuery TblBase::select(const TColumn &column, const WhereCondition &where, const QString &orderBy) const
{
    return select(TColumn::List() << column, where, orderBy);
}

QSqlQuery TblBase::select(const TColumn::List &columns, const WhereCondition &where, const QString &orderBy) const
{
    QStringList colsNames;
    for (auto column : columns) {
        colsNames << column.name;
    }
    return executeSelect(colsNames, where, orderBy);
}

bool TblBase::hasAnyRecord(const WhereCondition &where) const
{
    auto query = SqlQueryConstructor::selectOneQuery(tableName(), where);
    auto result = startQuery(query);
    if (!result.next()) {
        return false;
    }

    auto count = result.record().value("COUNT(*)").toInt();
    return count > 0;
}

QSqlQuery TblBase::executeSelect(const QStringList& cols, const WhereCondition& where, const QString& orderBy) const
{
    auto query = SqlQueryConstructor::selectQuery(tableName(), cols, where, orderBy);
    return startQuery(query);
}

QSqlQuery TblBase::executeInsert(const InsertContainer::List& values)
{
    auto query = SqlQueryConstructor::insertQuery(tableName(), values);
    return startQuery(query);
}

void TblBase::executeUpdate(const SetExpression& set, const WhereCondition& where)
{
    auto query = SqlQueryConstructor::updateQuery(tableName(), set, where);
    startQuery(query);
}

UuidList TblBase::filterEmptyUuids(const UuidList& uuids)
{
    UuidList ret;
    for (auto& uuid : uuids) {
        if (!uuid.isNull()) {
            ret.push_back(uuid);
        }
    }

    return ret;
}

void TblBase::updateWhere(const SetExpression& set, const WhereCondition& where)
{
    executeUpdate(set, where);
}

void TblBase::deleteRecord(const QUuid &uuid)
{
    auto query = SqlQueryConstructor::deleteByUuidQuery(tableName(), uuid);
    startQuery(query);
}

void TblBase::deleteWhere(const WhereCondition &where)
{
    auto query = SqlQueryConstructor::deleteWhereQuery(tableName(), where);
    startQuery(query);
}

bool TblBase::isColumnExist(const TColumn &column) const
{
    for (auto col : getAllColumns()) {
        if (col == column) {
            return true;
        }
    }

    return false;
}

void TblBase::checkCols()
{
    for (auto column : getAllColumns()) {
        auto query = SqlQueryConstructor::addColumn(tableName(), column);
        startQuery(query);
    }
}

void TblBase::setField(const TColumn &column, const QUuid &uuid, const QString &val)
{
    SetExpression set;
    set.set(column, val);

    updateWhere(set, WhereCondition::uuidEqual(uuid));
}

RecVector TblBase::toRecVector(QSqlQuery&& q)
{
    RecVector ret;

    if (auto size = q.size(); size > 0) {
        ret.reserve(size);
    }

    for (;;) {
        if (!q.next()) {
            break;
        }

        ret << q.record();
    }

    return ret;
}
