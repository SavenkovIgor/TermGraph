/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

#include "source/databaseWorks/tblbase.h"

#include "source/databaseWorks/tools/dbtools.h"

QString TblBase::getStringField(const TColumn& column, const QString& key) const
{
    assert(isColumnExist(column));

    QSqlQuery sel = select(column, primaryKeyEqual(key));

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

QSqlQuery TblBase::select(const TColumn& column, const WhereCondition& where, const QString& orderBy) const
{
    return select(TColumn::List() << column, where, orderBy);
}

QSqlQuery TblBase::select(const TColumn::List& columns, const WhereCondition& where, const QString& orderBy) const
{
    QStringList colsNames;

    for (const auto& column : columns)
        colsNames << column.name;

    return executeSelect(colsNames, where, orderBy);
}

bool TblBase::hasAnyRecord(const WhereCondition& where) const
{
    auto query  = SqlQueryConstructor::selectOneQuery(tableName(), where);
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

WhereCondition TblBase::primaryKeyEqual(const QString& value) const
{
    return WhereCondition::columnEqual(primaryKey(), value);
}

void TblBase::deleteByKey(const QString& key)
{
    auto query = SqlQueryConstructor::deleteWhereQuery(tableName(), primaryKeyEqual(key));
    startQuery(query);
}

void TblBase::deleteWhere(const WhereCondition& where)
{
    auto query = SqlQueryConstructor::deleteWhereQuery(tableName(), where);
    startQuery(query);
}

bool TblBase::isColumnExist(const TColumn& column) const
{
    for (const auto& col : getAllColumns())
        if (col == column)
            return true;

    return false;
}

void TblBase::setField(const TColumn& column, const QString& key, const QString& val)
{
    SetExpression set;
    set.set(column, val);

    updateWhere(set, primaryKeyEqual(key));
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