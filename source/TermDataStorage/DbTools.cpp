// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include "source/TermDataStorage/SqlQueryBuilder.h"

export module DbTools;

export class DbTools
{
public:
    using RecordList = std::vector<QSqlRecord>;

    // Transaction
    static void startTransaction(QSqlDatabase* base) { startQuery(base, "BEGIN TRANSACTION"); }
    static void endTransaction(QSqlDatabase* base) { startQuery(base, "END TRANSACTION"); }

    // Database
    static void reduceSpace(QSqlDatabase* base) { startQuery(base, "VACUUM"); }

    // Query
    static QSqlQuery startQuery(QSqlDatabase* base, const QString& queryString)
    {
        assert(base != nullptr);
        assert(!queryString.simplified().isEmpty());

        auto ret = QSqlQuery(queryString, *base);
        ret.exec();

        Q_ASSERT_X(!ret.lastError().isValid(),
                   Q_FUNC_INFO,
                   QString("Query %1\nfails with error %2")
                       .arg(queryString, ret.lastError().text())
                       .toStdString()
                       .c_str());

        return ret;
    }

    static void start(QSqlQuery& query)
    {
        if (!query.exec()) {
            qWarning() << "QSqlQuiery error: " << query.lastError().text();
            assert(false);
        }
    }

    static void start(QSqlQuery&& query)
    {
        if (!query.exec()) {
            qWarning() << "QSqlQuiery error: " << query.lastError().text();
            assert(false);
        }
    }

    // Common table statistic
    static int recordsCount(const QString& tableName)
    {
        auto query = SqlQueryBuilder().recordsCount(tableName);
        start(query);

        query.next();
        return query.value("COUNT(*)").toInt();
    }

    static QSqlRecord getRecord(QSqlQuery&& q)
    {
        [[maybe_unused]] auto nextValid = q.next();
        assert(nextValid);

        return q.record();
    }

    static RecordList getAllRecords(QSqlQuery&& q)
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
};
