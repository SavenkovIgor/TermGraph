#include "tblbase.h"

void TblBase::setField(QString columnName, QUuid uuid, QString val)
{
    SetExpression set;
    set.set(columnName, val);

    updateWhere(set, WhereCondition::uuidEqual(uuid));
}

int TblBase::getIntField(QString columnName, QUuid uuid)
{
    if( !isColumnNameExist(columnName) )
        return -1;

    QSqlQuery sel = select(QStringList() << columnName,
                           WhereCondition::uuidEqual(uuid));

    if(!sel.next())
        return -1;

    return sel.record().value(columnName).toInt();
}

QString TblBase::getStringField(QString columnName, QUuid uuid)
{
    if( !isColumnNameExist(columnName) )
        return "";

    QSqlQuery sel = select(QStringList() << columnName,
                           WhereCondition::uuidEqual(uuid));

    if(!sel.next())
        return "";

    return sel.record().value(columnName).toString();
}

QStringList TblBase::getAllCols()
{
    QStringList lst;
    for(TableColumnDescription column : columns) {
        lst << column.name;
    }
    return lst;
}

QSqlQuery TblBase::startQuery(QString str)
{
    //    qDebug()<<str;
    if (str.simplified().isEmpty())
        return QSqlQuery();

    QSqlQuery ret = base->exec(str);

    if(hasErrors(ret.lastError().text())) {
        qDebug()<<"Query "<<str<<"\nfails with error"<<ret.lastError().text();
    }

    return ret;
}

bool TblBase::hasErrors(QString errString)
{
    bool ret;
    ret = (errString != "" && errString != " ");
    return ret;
}

TblBase::TblBase(QString tblName, QSqlDatabase *base):
    tableName(tblName),
    base(base)
{
    queryConstructor = new SqlQueryConstructor(tblName);
}

QSqlQuery TblBase::createTable() {
    QString query = queryConstructor->createTable(columns);
    return startQuery(query);
}

void TblBase::initColumn(QString columnName, QString columnType) {
    TableColumnDescription column;
    column.name = columnName;
    column.type = columnType;
    columns.append(column);
}

bool TblBase::insertInto(QList<InsertContainer> values)
{
    QSqlQuery q = executeInsert(values);

    if( hasErrors( q.lastError().text() ) )
        return false;
    return true;
}

QSqlQuery TblBase::select(QStringList cols, WhereCondition where, QString orderBy)
{
    return executeSelect(cols, where, orderBy);
}

QSqlQuery TblBase::executeSelect(QStringList cols, WhereCondition where, QString orderBy)
{
    QString query = queryConstructor->selectQuery(cols,where,orderBy);
    return startQuery(query);
}

QSqlQuery TblBase::executeInsert(QList<InsertContainer> values)
{
    QString query = queryConstructor->insertQuery(values);
    return startQuery(query);
}

void TblBase::executeUpdate(SetExpression set, WhereCondition where)
{
    QString query = queryConstructor->updateQuery(set,where);
    startQuery(query);
}



void TblBase::updateWhere(SetExpression set, WhereCondition where)
{
    executeUpdate(set, where);
}

void TblBase::deleteRecord(QUuid uuid)
{
    QString query = queryConstructor->deleteByUuidQuery(uuid);
    startQuery(query);
}

void TblBase::deleteWhere(WhereCondition where)
{
    QString query = queryConstructor->deleteWhereQuery(where);
    startQuery(query);
}

bool TblBase::isColumnNameExist(QString columnName)
{
    for(TableColumnDescription col : columns) {
        if (col.name == columnName)
            return true;
    }
    return false;
}

void TblBase::checkCols()
{
    for( TableColumnDescription column : columns ) {
        QString query = queryConstructor->addColumn(column);
        startQuery(query);
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
