#include "tblbase.h"

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
    if (!isColumnExist(column)) {
        return "";
    }

    QSqlQuery sel = select(column, WhereCondition::uuidEqual(uuid));

    if (!sel.next()) {
        return "";
    }

    return sel.record().value(column).toString();
}

QSqlQuery TblBase::startQuery(const QString& queryString) const
{
//    qDebug() << queryString;
    if (queryString.simplified().isEmpty()) {
        return QSqlQuery();
    }

    QSqlQuery ret = base->exec(queryString);

    if (hasErrors(ret.lastError().text())) {
        qDebug() << "Query " << queryString << "\nfails with error" << ret.lastError().text();
    }

    return ret;
}

bool TblBase::hasErrors(const QString& errString) const
{
    bool ret;
    ret = (errString != "" && errString != " ");
    return ret;
}

TblBase::TblBase(QString tableName, QSqlDatabase *base):
    base(base),
    queryConstructor(new SqlQueryConstructor(tableName))
{
}

QSqlQuery TblBase::createTable() {
    QString query = queryConstructor->createTable(getAllColumns());
    return startQuery(query);
}

bool TblBase::insertInto(const QList<InsertContainer>& values)
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
    QString query = queryConstructor->selectOneQuery(where);
    auto result = startQuery(query);
    if (!result.next()) {
        return false;
    }

    auto count = result.record().value("COUNT(*)").toInt();
    return count > 0;
}

QSqlQuery TblBase::executeSelect(const QStringList& cols, const WhereCondition& where, const QString& orderBy) const
{
    QString query = queryConstructor->selectQuery(cols, where, orderBy);
    return startQuery(query);
}

QSqlQuery TblBase::executeInsert(const QList<InsertContainer>& values)
{
    QString query = queryConstructor->insertQuery(values);
    return startQuery(query);
}

void TblBase::executeUpdate(const SetExpression& set, const WhereCondition& where)
{
    QString query = queryConstructor->updateQuery(set,where);
    startQuery(query);
}

void TblBase::updateWhere(const SetExpression& set, const WhereCondition& where)
{
    executeUpdate(set, where);
}

void TblBase::deleteRecord(const QUuid &uuid)
{
    QString query = queryConstructor->deleteByUuidQuery(uuid);
    startQuery(query);
}

void TblBase::deleteWhere(const WhereCondition &where)
{
    QString query = queryConstructor->deleteWhereQuery(where);
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
        QString query = queryConstructor->addColumn(column);
        startQuery(query);
    }
}

void TblBase::setField(const TColumn &column, const QUuid &uuid, const QString &val)
{
    SetExpression set;
    set.set(column, val);

    updateWhere(set, WhereCondition::uuidEqual(uuid));
}

RecVector TblBase::toRecVector(QSqlQuery &&q)
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
