#include "dbtools.h"

#include <QSqlError>
//#include <QDebug>

#include "source/databaseWorks/sqlqueryconstructor.h"

void DbTools::startTransaction(QSqlDatabase *base)
{
    startQuery(base, "BEGIN TRANSACTION");
}

void DbTools::endTransaction(QSqlDatabase *base)
{
    startQuery(base, "END TRANSACTION");
}

void DbTools::reduceSpace(QSqlDatabase* base)
{
    startQuery(base, "VACUUM");
}

QSqlQuery DbTools::startQuery(QSqlDatabase* base, const QString& queryString)
{
//    qDebug() << queryString;
    assert(base != nullptr);
    assert(!queryString.simplified().isEmpty());

    QSqlQuery ret = base->exec(queryString);

    Q_ASSERT_X(!ret.lastError().isValid(),
               Q_FUNC_INFO,
               QString("Query " + queryString + "\nfails with error " + ret.lastError().text())
                   .toStdString()
                   .c_str());

    return ret;
}

int DbTools::recordsCount(QSqlDatabase* base, const QString& tableName)
{
    auto queryString = SqlQueryConstructor::recordsCount(tableName);
    auto query = startQuery(base, queryString);
    query.next();
    return query.value("COUNT(*)").toInt();
}
