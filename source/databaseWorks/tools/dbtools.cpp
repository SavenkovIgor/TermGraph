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

#include "source/databaseWorks/tools/dbtools.h"

#include <QSqlError>

#include "source/databaseWorks/sqlqueryconstructor.h"

void DbTools::startTransaction(QSqlDatabase* base)
{
    startQuery(base, "BEGIN TRANSACTION");
}

void DbTools::endTransaction(QSqlDatabase* base)
{
    startQuery(base, "END TRANSACTION");
}

void DbTools::reduceSpace(QSqlDatabase* base)
{
    startQuery(base, "VACUUM");
}

QSqlQuery DbTools::startQuery(QSqlDatabase* base, const QString& queryString)
{
    assert(base != nullptr);
    assert(!queryString.simplified().isEmpty());

    QSqlQuery ret = base->exec(queryString);

    Q_ASSERT_X(
        !ret.lastError().isValid(),
        Q_FUNC_INFO,
        QString("Query %1\nfails with error %2").arg(queryString).arg(ret.lastError().text()).toStdString().c_str());

    return ret;
}

int DbTools::recordsCount(QSqlDatabase* base, const QString& tableName)
{
    auto queryString = SqlQueryConstructor::recordsCount(tableName);
    auto query       = startQuery(base, queryString);
    query.next();
    return query.value("COUNT(*)").toInt();
}
