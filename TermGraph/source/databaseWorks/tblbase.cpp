#include "tblbase.h"

void TblBase::setField(const QString& columnName, const QUuid& uuid, const QString& val)
{
    SetExpression set;
    set.set(columnName, val);

    updateWhere(set, WhereCondition::uuidEqual(uuid));
}

int TblBase::getIntField(const QString &columnName, const QUuid &uuid) const
{
    if( !isColumnNameExist(columnName) )
        return -1;

    QSqlQuery sel = select(QStringList() << columnName,
                           WhereCondition::uuidEqual(uuid));

    if(!sel.next())
        return -1;

    return sel.record().value(columnName).toInt();
}

QString TblBase::getStringField(const QString& columnName, const QUuid& uuid) const
{
    if( !isColumnNameExist(columnName) )
        return "";

    QSqlQuery sel = select(QStringList() << columnName,
                           WhereCondition::uuidEqual(uuid));

    if(!sel.next())
        return "";

    return sel.record().value(columnName).toString();
}

QStringList TblBase::getAllCols() const
{
    QStringList lst;
    for(TableColumnDescription column : columns) {
        lst << column.name;
    }
    return lst;
}

QSqlQuery TblBase::startQuery(const QString& queryString) const
{
    //    qDebug()<<str;
    if (queryString.simplified().isEmpty())
        return QSqlQuery();

    QSqlQuery ret = base->exec(queryString);

    if(hasErrors(ret.lastError().text())) {
        qDebug() << "Query " << queryString << "\nfails with error" << ret.lastError().text();
    }

    return ret;
}

bool TblBase::hasErrors(const QString& errString) const
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

TblBase::~TblBase()
{
    delete queryConstructor;
}

QSqlQuery TblBase::createTable() {
    QString query = queryConstructor->createTable(columns);
    return startQuery(query);
}

void TblBase::initColumn(const QString& columnName, const QString& columnType) {
    TableColumnDescription column;
    column.name = columnName;
    column.type = columnType;
    columns.append(column);
}

bool TblBase::insertInto(const QList<InsertContainer>& values)
{
    QSqlQuery q = executeInsert(values);

    if( hasErrors( q.lastError().text() ) )
        return false;
    return true;
}

QSqlQuery TblBase::select(const QStringList& cols, const WhereCondition& where, const QString& orderBy) const
{
    return executeSelect(cols, where, orderBy);
}

QSqlQuery TblBase::executeSelect(const QStringList& cols, const WhereCondition& where, const QString& orderBy) const
{
    QString query = queryConstructor->selectQuery(cols,where,orderBy);
    return startQuery(query);
}

QSqlQuery TblBase::executeInsert(const QList<InsertContainer>& values)
{
    QString query = queryConstructor->insertQuery(values);
    return startQuery(query);
}

void TblBase::executeUpdate(const SetExpression& set, const WhereCondition& where)
{
    QString query = queryConstructor->updateQuery(set,where);
    startQuery(query);
}

void TblBase::updateWhere(const SetExpression& set, const WhereCondition& where)
{
    executeUpdate(set, where);
}

void TblBase::deleteRecord(const QUuid &uuid)
{
    QString query = queryConstructor->deleteByUuidQuery(uuid);
    startQuery(query);
}

void TblBase::deleteWhere(const WhereCondition &where)
{
    QString query = queryConstructor->deleteWhereQuery(where);
    startQuery(query);
}

bool TblBase::isColumnNameExist(const QString& columnName) const
{
    for(auto col : columns) {
        if (col.name == columnName) {
            return true;
        }
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
