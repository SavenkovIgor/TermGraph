// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/TermDataStorage/DbTools.h"

#include <QSqlError>

#include "source/TermDataStorage/SqlQueryBuilder.h"

void DbTools::startTransaction(QSqlDatabase* base) { startQuery(base, "BEGIN TRANSACTION"); }

void DbTools::endTransaction(QSqlDatabase* base) { startQuery(base, "END TRANSACTION"); }

void DbTools::reduceSpace(QSqlDatabase* base) { startQuery(base, "VACUUM"); }

QSqlQuery DbTools::startQuery(QSqlDatabase* base, const QString& queryString)
{
    assert(base != nullptr);
    assert(!queryString.simplified().isEmpty());

    auto ret = QSqlQuery(queryString, *base);
    ret.exec();

    Q_ASSERT_X(!ret.lastError().isValid(),
               Q_FUNC_INFO,
               QString("Query %1\nfails with error %2").arg(queryString, ret.lastError().text()).toStdString().c_str());

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
    [[maybe_unused]] auto nextValid = q.next();
    assert(nextValid);

    return q.record();
}

DbTools::RecordList DbTools::getAllRecords(QSqlQuery&& q)
{
    DbTools::RecordList ret;

    if (auto size = q.size(); size > 0) {
        ret.reserve(static_cast<DbTools::RecordList::size_type>(size));
    }

    for (;;) {
        if (!q.next()) {
            break;
        }

        ret.push_back(q.record());
    }

    return ret;
}
