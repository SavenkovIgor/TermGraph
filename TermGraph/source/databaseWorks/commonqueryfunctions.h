#ifndef COMMONQUERYFUNCTIONS_H
#define COMMONQUERYFUNCTIONS_H

#include <QString>

class CommonQueryFunctions {
public:
    constexpr static auto joinParam = ", ";

    static QString vv(QString str)
    {
        str.replace("'","''");
        str.replace("\"","\"\"");
        QString ret = "'" + str + "'";
        return ret;
    }
};

#endif // COMMONQUERYFUNCTIONS_H
