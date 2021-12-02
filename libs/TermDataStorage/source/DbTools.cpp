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

#include "source/DbTools.h"

#include <QSqlError>

#include "source/SqlQueryBuilder.h"

void DbTools::startTransaction(QSqlDatabase* base) { startQuery(base, "BEGIN TRANSACTION"); }

void DbTools::endTransaction(QSqlDatabase* base) { startQuery(base, "END TRANSACTION"); }

void DbTools::reduceSpace(QSqlDatabase* base) { startQuery(base, "VACUUM"); }

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

void DbTools::start(QSqlQuery& query)
{
    if (!query.exec()) {
        qWarning() << "QSqlQuiery error: " << query.lastError().text();
        assert(false);
    }
}

void DbTools::start(QSqlQuery&& query)
{
    if (!query.exec()) {
        qWarning() << "QSqlQuiery error: " << query.lastError().text();
        assert(false);
    }
}

int DbTools::recordsCount(const QString& tableName)
{
    auto query = SqlQueryBuilder().recordsCount(tableName);
    start(query);

    query.next();
    return query.value("COUNT(*)").toInt();
}

QSqlRecord DbTools::getRecord(QSqlQuery&& q)
{
    auto nextValid = q.next();
    assert(nextValid);

    return q.record();
}

RecordList DbTools::getAllRecords(QSqlQuery&& q)
{
    RecordList ret;

    if (auto size = q.size(); size > 0) {
        ret.reserve(size);
    }

    for (;;) {
        if (!q.next()) {
            break;
        }

        ret.push_back(q.record());
    }

    return ret;
}
