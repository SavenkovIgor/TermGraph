#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>

class DbTools
{
public:
    static void startTransaction(QSqlDatabase* base);
    static void endTransaction(QSqlDatabase* base);

    static QSqlQuery startQuery(QSqlDatabase* base, const QString& queryString);
};
