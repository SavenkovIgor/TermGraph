/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
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

#pragma once

#include <QtSql>

#include "source/Database/sqlqueryconstructor.h"
#include "source/Helpers/handytypes.h"

using SqCond    = QMap<QString, QString>;
using RecVector = QVector<QSqlRecord>;

class TblBase
{
public:
    TblBase(QSqlDatabase* base);
    virtual ~TblBase() = default;

protected:
    QString getStringField(const TColumn& column, const QString& key) const;

    QSqlQuery select(const TColumn&        column,
                     const WhereCondition& where   = WhereCondition(),
                     const QString&        orderBy = "") const;

    QSqlQuery select(const TColumn::List&  columns,
                     const WhereCondition& where   = WhereCondition(),
                     const QString&        orderBy = "") const;

    bool insertInto(const InsertContainer::List& values);

    WhereCondition primaryKeyEqual(const QString& value) const;

    static RecVector toRecVector(QSqlQuery&& q);

    virtual const char*   tableName() const     = 0;
    virtual TColumn       primaryKey() const    = 0;
    virtual TColumn::List getAllColumns() const = 0;
    virtual void          initTable()           = 0;

    static UuidList filterEmptyUuids(const UuidList& uuids);

protected:
    void startQuery(QSqlQuery query);

private:
    QSqlDatabase* base;
    bool          isColumnExist(const TColumn& column) const;

    QSqlQuery startQuery(const QString& queryString) const;
    bool      hasErrors(const QString& errString) const;

    QSqlQuery executeSelect(const QStringList& cols, const WhereCondition& where, const QString& orderBy = "") const;
    QSqlQuery executeInsert(const InsertContainer::List& values);
};
