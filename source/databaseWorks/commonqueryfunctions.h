#ifndef COMMONQUERYFUNCTIONS_H
#define COMMONQUERYFUNCTIONS_H

class CommonQueryFunctions {
public:
    static QString joinParam() {
        return ", ";
    }

    static QString vv(QString str)
    {
        str.replace("'","''");
        str.replace("\"","\"\"");
        QString ret = "'" + str + "'";
        return ret;
    }
};

#endif // COMMONQUERYFUNCTIONS_H
