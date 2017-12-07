#ifndef TBLBASE_H
#define TBLBASE_H

#include <QObject>
#include <QtSql>

typedef QMap <QString, QString> SqCond;
typedef QList<QSqlRecord>       RecList;

class TblBase{    
public:
    TblBase(QString tblName,QSqlDatabase *base);

    QSqlQuery createTable();
    void checkCols();

protected:
    void    setField(QString field,int uid,QString val);

    int     getIntField   (QString field,int uid);
    QString getStringField(QString field,int uid);

    void addCol( QString colName, QString colType );
    QStringList getAllCols();

    bool insertInto(QStringList cols, QStringList vals);
    QSqlQuery select(QStringList cols, QString where = "", QString add = "");
    QSqlQuery select(QString     cols, QString where = "");
    QSqlQuery select(QStringList cols, SqCond where);

    QString vv(QString str);

    QSqlQuery updateWhere(SqCond set, SqCond where);
    QSqlQuery updateWhere(SqCond set, QString where);

    RecList toRecList(QSqlQuery q);

    void deleteRecord(int id);
    void deleteWhere(QString where);

private:
    QSqlDatabase *base;

    QString tblName;
    QStringList allColumnNames;
    QMap<QString, QString> allColumnTypes;


    QSqlQuery start(QString str);
    bool hasErrors(QString errString);

    QStringList parseWhere(SqCond where);

    QSqlQuery z_sl(QString cols, QString where, QString add = "");
    QSqlQuery z_in(QString cols, QString vals);
    QSqlQuery z_up(QString set, QString where);

};

#endif // TBLBASE_H
