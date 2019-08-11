#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>

class DbTools
{
public:
    // Transaction
    static void startTransaction(QSqlDatabase* base);
    static void endTransaction(QSqlDatabase* base);

    // Query
    static QSqlQuery startQuery(QSqlDatabase* base, const QString& queryString);

    // Common table statistic
    static int recordsCount(QSqlDatabase* base, const QString& tableName);
};
