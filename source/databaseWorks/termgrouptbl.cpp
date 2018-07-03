#include "termgrouptbl.h"

bool TermGroupTbl::addGroup(QString name, QString comment, int type)
{
    if(name.simplified() == "")
        return false;

    QUuid uuid;

    for( int i = 0; i < 1000; i++ ) {
        uuid = QUuid::createUuid();
        if(!isUuidExist(uuid.toString())) {
            if( i == 0)
                qDebug()<<"break at 0";
            break;
        }
    }

    return addGroup(uuid.toString(), name, comment, type);
}

bool TermGroupTbl::addGroup(QString uuid, QString name, QString comment, int type)
{
    if(uuid.simplified() == "")
        return false;

    if(name.simplified() == "")
        return false;

    QList<InsertContainer> values;
    values << InsertContainer(this->longUID, uuid);
    values << InsertContainer(this->name, name);
    values << InsertContainer(this->comment, comment);
    values << InsertContainer(this->type, type);

    return insertInto(values);
}

QList<int> TermGroupTbl::getAllGroupsUid()
{
    QList<int> ret;

    QSqlQuery q = select(QStringList() << this->uid, WhereConditions(), "uid");

    for(int i=0;i<1000000;i++) {
        if(!q.next())
            break;

        ret << q.record().value(this->uid).toInt();
    }

    return ret;
}

QList<QUuid> TermGroupTbl::getAllGroupsUuid()
{
    QList<QUuid> ret;

    QSqlQuery q = select(QStringList() << this->longUID, WhereConditions(), this->longUID);

    for(int i = 0; i < 1000000; i++) {
        if(!q.next())
            break;

        QUuid grpUuid = QUuid(q.record().value(this->longUID).toString());
        if( !grpUuid.isNull() )
            ret << grpUuid;
    }

    return ret;
}

void TermGroupTbl::deleteGroup(QString name)
{
    WhereConditions where;
    where.equal(this->name,name);
    deleteWhere(where);
}

bool TermGroupTbl::isUuidExist(QString longUuid)
{
    WhereConditions where;
    where.equal(this->longUID,longUuid);

    RecList recs = toRecList(select(QStringList() << this->longUID, where));
    return !recs.isEmpty();
}

void TermGroupTbl::normalizeUuid()
{
    QList<int> grpLst = getAllGroupsUid();
    int found = 0;
    for( int &i : grpLst ) {

        QSqlRecord rec = getGroup(i);
        if( rec.value( this->longUID ).toString() == "" ) {
            setField(this->longUID,i,QUuid::createUuid().toString());
            found++;
        }
    }
    if( found > 0 ){
        qDebug()<<"Found " + QString::number( found ) + " nodes for Uuid normalization";
    } else {
        qDebug()<<"Nothing to normalize";
    }
}

int TermGroupTbl::getUid(QString groupName)
{
    WhereConditions where;
    where.equal(this->name, groupName);

    QSqlQuery q = select(QStringList() << this->uid, where);
    if(!q.next())
        return -1;

    return q.record().value(this->uid).toInt();
}

bool TermGroupTbl::isGroupExistWithUid(QString uid)
{
    return getUid(uid) != -1;
}

bool TermGroupTbl::isGroupExistWithLongUid(QString longUid)
{
    WhereConditions where;
    where.equal(this->longUID, longUid);

    QSqlQuery q = select(QStringList()<<this->longUID, where);

    if(!q.next())
        return false;

    return true;
}

QString TermGroupTbl::getName(int groupUid)
{
    return getStringField(this->name, groupUid);
}

QString TermGroupTbl::getLongUid(int groupUid)
{
    return getStringField(this->longUID, groupUid);
}

int TermGroupTbl::getType(int groupUid)
{
    return getIntField( this->type, groupUid );
}

void TermGroupTbl::setName(QString uuid, QString name)
{
    setField(this->name, uuid, name);
}

void TermGroupTbl::setComment(QString uuid, QString comment)
{
    setField(this->comment, uuid, comment);
}

void TermGroupTbl::setType(QString uuid, int type)
{
    setField(this->type, uuid, QString::number(type));
}

QStringList TermGroupTbl::getAllGroupsNames(QString area, bool withUid)
{
    QStringList ret;

    QSqlQuery q = select(QStringList() << this->name << this->uid, WhereConditions(), "uid");

    for(int i=0;i<1000000;i++) {
        if(!q.next())
            break;

        if( withUid )
            ret << q.record().value(this->name).toString() + " (" + q.record().value(this->uid).toString() + ")";
        else
            ret << q.record().value(this->name).toString();
    }

    return ret;
}

QSqlRecord TermGroupTbl::getGroup(int id)
{
    WhereConditions where;
    where.equal(this->uid,id);

    QSqlQuery sel = select(getAllCols(), where);

    if(!sel.next())
        return QSqlRecord();

    return sel.record();
}

QSqlRecord TermGroupTbl::getGroup(QUuid uuid)
{
    WhereConditions where;
    where.equal(this->longUID,uuid.toString());

    QSqlQuery sel = select(getAllCols(), where);

    if(!sel.next())
        return QSqlRecord();

    return sel.record();
}
