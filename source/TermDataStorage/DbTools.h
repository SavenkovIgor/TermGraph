// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

#include <source/CommonTools/HandyTypes.h>

class DbTools
{
public:
    using RecordList = std::vector<QSqlRecord>;

    // Transaction
    static void startTransaction(QSqlDatabase* base);
    static void endTransaction(QSqlDatabase* base);

    // Database
    static void reduceSpace(QSqlDatabase* base);

    // Query
    static QSqlQuery startQuery(QSqlDatabase* base, const QString& queryString);
    static void      start(QSqlQuery& query);
    static void      start(QSqlQuery&& query);

    // Common table statistic
    static int recordsCount(const QString& tableName);

    static QSqlRecord getRecord(QSqlQuery&& q);
    static RecordList getAllRecords(QSqlQuery&& q);
};
