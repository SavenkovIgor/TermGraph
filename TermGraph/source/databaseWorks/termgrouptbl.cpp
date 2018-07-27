#include "termgrouptbl.h"

const QString TermGroupTbl::uid = "uid";

bool TermGroupTbl::addGroup(QString name, QString comment, int type)
{
    if(name.simplified() == "")
        return false;

    QUuid uuid = generateNewUuid();
    return addGroup(uuid, name, comment, type);
}

bool TermGroupTbl::addGroup(QUuid uuid, QString name, QString comment, int type)
{
    if(uuid.isNull())
        return false;

    if(name.simplified() == "")
        return false;

    QList<InsertContainer> values;
    values << InsertContainer(this->longUID, uuid.toString());
    values << InsertContainer(this->name, name);
    values << InsertContainer(this->comment, comment);
    values << InsertContainer(this->type, type);

    return insertInto(values);
}

QList<QUuid> TermGroupTbl::getAllGroupsUuid()
{
    QList<QUuid> ret;
    RecList records = toRecList(select(QStringList() << this->longUID));

    for(QSqlRecord record: records) {
        QUuid uuid(record.value(this->longUID).toString());
        if(!uuid.isNull())
            ret << uuid;
    }

    return ret;
}

void TermGroupTbl::deleteGroup(QString name)
{
    WhereCondition where;
    where.equal(this->name, name);
    deleteWhere(where);
}

bool TermGroupTbl::isUuidExist(QUuid uuid)
{
    RecList recs = toRecList(select(QStringList() << this->longUID,
                                    WhereCondition::uuidEqual(uuid)));
    return !recs.isEmpty();
}

QUuid TermGroupTbl::generateNewUuid()
{
    QUuid uuid;
    for( int i = 0; i < 1000; i++ ) {
        uuid = QUuid::createUuid();
        if(!isUuidExist(uuid)){
            break;
        }
    }
    return uuid;
}

QUuid TermGroupTbl::getUuid(QString groupName)
{
    WhereCondition where;
    where.equal(this->name, groupName);

    QSqlQuery q = select(QStringList() << this->longUID, where);
    if(!q.next())
        return QUuid();

    return QUuid(q.record().value(this->longUID).toString());
}

bool TermGroupTbl::hasGroupWithName(QString groupName)
{
    return !getUuid(groupName).isNull();
}

int TermGroupTbl::getType(QUuid groupUuid)
{
    return getIntField(this->type, groupUuid);
}

void TermGroupTbl::setName(QUuid uuid, QString name)
{
    setField(this->name, uuid, name);
}

void TermGroupTbl::setComment(QUuid uuid, QString comment)
{
    setField(this->comment, uuid, comment);
}

void TermGroupTbl::setType(QUuid uuid, int type)
{
    setField(this->type, uuid, QString::number(type));
}

QString TermGroupTbl::getName(QUuid uuid)
{
    return getStringField(this->name, uuid);
}

QStringList TermGroupTbl::getAllGroupsNames(QString area, bool withUid)
{
    QStringList ret;

    QSqlQuery q = select(QStringList() << this->name << this->uid, WhereCondition(), "uid");

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

QSqlRecord TermGroupTbl::getGroup(QUuid uuid)
{
    QSqlQuery sel = select(getAllCols(), WhereCondition::uuidEqual(uuid));

    if(!sel.next())
        return QSqlRecord();

    return sel.record();
}
