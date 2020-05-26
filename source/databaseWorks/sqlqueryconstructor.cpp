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

#include "source/databaseWorks/sqlqueryconstructor.h"
#include "source/databaseWorks/tools/querytools.h"

QString SqlQueryConstructor::createTable(const QString& tableName, const TColumn::List& columns)
{
    QStringList colsDescription;
    for (const auto& column : columns)
        colsDescription << QString(column.name) + " " + QString(column.type);

    QStringList qry;

    qry << "CREATE TABLE";
    qry << tableName;
    qry << "(" + colsDescription.join(QueryTools::joinParam) + ")";

    return qry.join(" ");
}

QString SqlQueryConstructor::addColumn(const QString& tableName, const TColumn& column)
{
    QStringList qry;
    qry << "ALTER TABLE";
    qry << tableName;
    qry << "ADD COLUMN";
    qry << column.name;
    qry << column.type;

    return qry.join(" ");
}

QString SqlQueryConstructor::dropTable(const QString& tableName)
{
    return "DROP TABLE " + tableName;
}

QString SqlQueryConstructor::recordsCount(const QString& tableName)
{
    return "SELECT COUNT(*) FROM " + tableName;
}

QString SqlQueryConstructor::selectQuery(const QString&        tableName,
                                         const QStringList&    columns,
                                         const WhereCondition& where,
                                         const QString&        orderBy)
{
    QStringList qry;
    qry << "SELECT";
    qry << columns.join(QueryTools::joinParam);
    qry << "FROM";
    qry << tableName;
    if (where.getJoinedConditions() != "") {
        qry << "WHERE";
        qry << where.getJoinedConditions();
    }
    if (orderBy.simplified() != "") {
        qry << "ORDER BY";
        qry << orderBy;
    }

    return qry.join(" ");
}

QString SqlQueryConstructor::selectOneQuery(const QString& tableName, const WhereCondition& where)
{
    QStringList qry;
    qry << "SELECT COUNT(*) FROM";
    qry << tableName;
    if (where.getJoinedConditions() != "") {
        qry << "WHERE";
        qry << where.getJoinedConditions();
    }

    qry << "LIMIT 1";

    return qry.join(" ");
}

QString SqlQueryConstructor::insertQuery(const QString& tableName, const InsertContainer::List& values)
{
    QStringList columns;
    QStringList insertValues;

    for (const auto& value : values) {
        columns << value.getColumnName();
        insertValues << QueryTools::vv(value.getValue());
    }

    QStringList qry;
    qry << "INSERT INTO";
    qry << tableName;
    qry << "( " + columns.join(QueryTools::joinParam) + " )";
    qry << "VALUES";
    qry << "( " + insertValues.join(QueryTools::joinParam) + " )";

    return qry.join(" ");
}

QString SqlQueryConstructor::updateQuery(const QString& tableName, const SetExpression& set, const WhereCondition& where)
{
    QStringList qry;
    qry << "UPDATE";
    qry << tableName;
    qry << "SET";
    qry << set.getExpression();
    qry << "WHERE";
    qry << where.getJoinedConditions();

    return qry.join(" ");
}

QString SqlQueryConstructor::deleteWhereQuery(const QString& tableName, const WhereCondition& where)
{
    QStringList qry;
    qry << "DELETE FROM";
    qry << tableName;
    qry << "WHERE";
    qry << where.getJoinedConditions();

    return qry.join(" ");
}
