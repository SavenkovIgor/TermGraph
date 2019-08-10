#include "dbtools.h"

#include <QSqlError>
//#include <QDebug>

void DbTools::startTransaction(QSqlDatabase *base)
{
    startQuery(base, "BEGIN TRANSACTION");
}

void DbTools::endTransaction(QSqlDatabase *base)
{
    startQuery(base, "END TRANSACTION");
}

QSqlQuery DbTools::startQuery(QSqlDatabase* base, const QString& queryString)
{
//    qDebug() << queryString;
    Q_ASSERT(base != nullptr);
    Q_ASSERT(!queryString.simplified().isEmpty());

    QSqlQuery ret = base->exec(queryString);

    Q_ASSERT_X(!ret.lastError().isValid(),
               Q_FUNC_INFO,
               QString("Query " + queryString + "\nfails with error " + ret.lastError().text())
                   .toStdString()
                   .c_str());

    return ret;
}
