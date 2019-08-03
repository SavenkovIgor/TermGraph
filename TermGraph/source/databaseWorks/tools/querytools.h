#pragma once

#include <QString>
#include <QStringList>

class QueryTools
{
public:
    constexpr static auto joinParam = ", ";

    static QString vv(QString str)
    {
        str.replace("'", "''");
        str.replace("\"", "\"\"");
        QString ret = "'" + str + "'";
        return ret;
    }

    static QStringList vv(QStringList lst)
    {
        for (QString& str : lst) {
            str = QueryTools::vv(str);
        }
        return lst;
    }
};
