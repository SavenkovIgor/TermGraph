#include "tblbase.h"


void TblBase::setField(QString field, int uid, QString val)
{
    SqCond set;
    set[field] = val;

    SqCond where;
    where["uid"] = QString::number(uid);

    updateWhere(set,where);
}

int TblBase::getIntField(QString field, int uid)
{
    if( !allColumnNames.contains(field) )
        return -1;

    SqCond where;
    where["uid"] = QString::number(uid);

    QSqlQuery sel = select(QStringList()<<field,where);

    if(!sel.next())
        return -1;

    return sel.record().value(field).toInt();
}

QString TblBase::getStringField(QString field, int uid)
{
    if( !allColumnNames.contains(field) )
        return "";

    SqCond where;
    where["uid"] = QString::number(uid);

    QSqlQuery sel = select(QStringList()<<field,where);

    if(!sel.next())
        return "";

    return sel.record().value(field).toString();
}

QStringList TblBase::getAllCols()
{
    return allColumnNames;
}

QSqlQuery TblBase::start(QString str)
{
    //    qDebug()<<str;
    QSqlQuery ret = base->exec(str);

    if(hasErrors(ret.lastError().text())) {
        qDebug()<<"Запрос "<<str<<" выпал с ошибкой "<<ret.lastError().text();
    }

    return ret;
}

bool TblBase::hasErrors(QString errString)
{
    bool ret;
    ret = (errString != "" && errString != " ");
    return ret;
}

QStringList TblBase::parseWhere(SqCond where)
{
    QStringList ret;

    for(QString s:where.keys()) {

        if(where[s].startsWith("!"))
            ret << s + " != " + vv(where[s].mid(1));

        else if(where[s] == "IS NULL")
            ret << s + " " + where[s];

        else if(where[s] == "IS NOT NULL")
            ret << s + " " + where[s];

        else
            ret << s + " = " + vv(where[s]);

    }

    return ret;
}

TblBase::TblBase(QString tblName, QSqlDatabase *base):
    tblName(tblName),
    base(base)
{

}

QSqlQuery TblBase::createTable() {
    QStringList lst;
    for( QString s:allColumnNames )
        lst << s + " " + allColumnTypes[s];

    return start("CREATE TABLE " + tblName + " (" + lst.join(", ") + ")" );
}

void TblBase::addCol(QString colName, QString colType) {
    allColumnTypes.insert(colName,colType);
    allColumnNames << colName;
}

bool TblBase::insertInto(QStringList cols, QStringList vals)
{
    for(int i=0;i<vals.size();i++)
        vals[i] = vv(vals[i]);

    QSqlQuery q = z_in(cols.join(", "),vals.join(", "));

    if( hasErrors( q.lastError().text() ) )
        return false;
    return true;
}

QSqlQuery TblBase::select(QStringList cols, QString where, QString add)
{
    return z_sl(cols.join(", "),where,add);
}

QSqlQuery TblBase::select(QString cols, QString where)
{
    return z_sl(cols,where);
}

QSqlQuery TblBase::select(QStringList cols, SqCond where)
{
    return z_sl(cols.join(", "),parseWhere(where).join(" AND "));
}

QSqlQuery TblBase::z_sl(QString cols, QString where, QString add)
{
    QStringList qry;
    qry << "SELECT";
    qry << cols;
    qry << "FROM";
    qry << tblName;
    if( where.simplified() != "" ){
        qry << "WHERE";
        qry << where;
    }
    qry<<add;

    return start(qry.join(" "));
}

QSqlQuery TblBase::z_in(QString cols, QString vals)
{
    QStringList qry;
    qry << "INSERT INTO";
    qry << tblName;
    qry << "(";
    qry << cols;
    qry << ") VALUES (";
    qry << vals;
    qry << ")";

    return start(qry.join(" "));
}

QSqlQuery TblBase::z_up(QString set, QString where)
{
    QStringList qry;
    qry << "UPDATE";
    qry << tblName;
    qry << "SET";
    qry << set;
    qry << "WHERE";
    qry << where;

    return start(qry.join(" "));
}

QString TblBase::vv(QString str)
{
    str.replace("'","''");
    str.replace("\"","\"\"");
    QString ret = "'" + str + "'";
    return ret;
}

QSqlQuery TblBase::updateWhere(SqCond set, SqCond where)
{
    QStringList setLst;

    for( QString s:set.keys() )
        setLst << s + " = " + vv(set[s]);

    QStringList whereLst;

    for( QString s:where.keys() )
        whereLst << s + " = " + vv(where[s]);

    return z_up(setLst.join(", "),whereLst.join(" AND "));
}

QSqlQuery TblBase::updateWhere(SqCond set, QString where)
{
    QStringList setLst;

    for( QString s:set.keys() ) {
        setLst << s + " = " + vv(set[s]);
    }

    return z_up(setLst.join(", "), where);
}

void TblBase::deleteRecord(int id)
{
    QStringList qry;
    qry << "DELETE";
    qry << "FROM";
    qry << tblName;
    qry << "WHERE";
    qry << "uid =";
    qry << vv(QString::number(id));

    start(qry.join(" "));
}

void TblBase::deleteWhere(QString where)
{
    QStringList qry;
    qry << "DELETE";
    qry << "FROM";
    qry << tblName;
    qry << "WHERE";
    qry << where;

    start(qry.join(" "));
}

void TblBase::checkCols()
{
    for( QString s : allColumnNames ) {
        start( "ALTER TABLE " + tblName + " ADD COLUMN " + s + " " + allColumnTypes[s]);
    }
}

RecList TblBase::toRecList(QSqlQuery q)
{
    RecList ret;
    for(int i=0;i<1000000;i++) {
        if(!q.next())
            break;
        ret << q.record();
    }
    return ret;
}
