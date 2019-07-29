#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>

class DbTools
{
public:
    static QSqlQuery startQuery(QSqlDatabase* base, const QString& queryString);
};
