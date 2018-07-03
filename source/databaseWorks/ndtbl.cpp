#include "ndtbl.h"

QUuid NdTbl::addNode(QString name)
{
    QUuid uuid = generateNewUuid();
    return addNode(uuid, name);
}

QUuid NdTbl::addNode(QUuid uuid, QString name)
{
    if( name.simplified() == "" )
        return ""; //Создать вершину не удалось

    QList<InsertContainer> values;
    values << InsertContainer(this->term, name);
    values << InsertContainer(this->longUID, uuid.toString());

    insertInto(values);

    updateLastEdit(uuid);
    return uuid;
}

int NdTbl::getRemindNum(int uid)
{
    QSqlRecord rec = getNode(uid);

    if( rec.isEmpty() )
        return -1;
    return rec.value(this->remindNum).toInt();
}

void NdTbl::setName(QUuid uuid, QString name)
{
    setField(this->term, uuid, name);
    updateLastEdit(uuid);
}

void NdTbl::setWordForms(QUuid uuid, QString forms)
{
    setField(this->termForms, uuid, forms);
    updateLastEdit(uuid);
}

void NdTbl::setDefinition(QUuid uuid, QString definition)
{
    setField(this->definition, uuid, definition);
    updateLastEdit(uuid);
}

void NdTbl::setDescription(QUuid uuid, QString description)
{
    setField(this->description, uuid, description);
    updateLastEdit(uuid);
}

void NdTbl::setExamples(QUuid uuid, QString example)
{
    setField(this->examples, uuid, example);
    updateLastEdit(uuid);
}

void NdTbl::setWikiRef(QUuid uuid, QString wikiRef)
{
    setField(this->wikiRef, uuid, wikiRef);
    updateLastEdit(uuid);
}

void NdTbl::setWikiImg(QUuid uuid, QString wikiImage)
{
    setField(this->wikiImg, uuid, wikiImage);
    updateLastEdit(uuid);
}

void NdTbl::setGroup(QUuid nodeUuid, QUuid groupUuid)
{
    setField(this->termGroup, nodeUuid, groupUuid.toString());
    updateLastEdit(nodeUuid);
}

void NdTbl::setAtLearn(QUuid uuid, bool learn)
{
    if( learn )
        setField(this->atLearn, uuid, "1");
    else
        setField(this->atLearn, uuid, "0");
}

//void NdTbl::setRemindToday(int uid)
//{
//    setRemindNum(uid, getRemindNum(uid) + 1, QDateTime::currentDateTime() );
//}

void NdTbl::setRemindNum(int uid, int num, QDate date)
{
    SetExpression set;
    set.set(this->remindNum, num);
    set.set(this->lastRemind, date.toString());

    WhereConditions where;
    where.equal(this->uid,uid);

    updateWhere(set,where);
}

void NdTbl::deleteNode(int id)
{
    deleteRecord(id);
}

void NdTbl::normalizeUuid()
{
    QList<int> ndLst = getAllNodesUid();
    int found = 0;
    for( int &i : ndLst ) {

        QSqlRecord rec = getNode(i);
        if( rec.value( this->longUID ).toString() == "" ) {
            setField(this->longUID, i, QUuid::createUuid().toString());
            found++;
        }
    }
    if( found > 0 ){
        qDebug()<<"Found " + QString::number( found ) + " nodes for Uuid normalization";
    } else {
        qDebug()<<"Nothing to normalize";
    }
}

void NdTbl::updateLastEdit(QUuid uuid)
{
    setField(this->lastEdit, uuid, QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
}

bool NdTbl::isUuidExist(QUuid uuid)
{
    WhereConditions where;
    where.equal(this->longUID, uuid.toString());

    RecList recs = toRecList(select(QStringList() << this->longUID, where));
    return !recs.isEmpty();
}

QUuid NdTbl::generateNewUuid()
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

QList<int> NdTbl::getAllNodesUid()
{
    QList<int> ret;

    RecList idS = toRecList(select(QStringList() << this->uid));
    for( QSqlRecord &r : idS )
        ret << r.value( this->uid ).toInt();

    return ret;
}

QList<QUuid> NdTbl::getAllNodesUuids()
{
    QList<QUuid> ret;

    RecList idS = toRecList(select(QStringList() << this->longUID));
    for( QSqlRecord &r : idS ) {
        QUuid tmpUuid(r.value( this->longUID ).toString());
        if(!tmpUuid.isNull())
            ret << tmpUuid;
    }

    return ret;
}

QList<int> NdTbl::getGroupNodeID(int groupID)
{
    QList<int> ret;

    if(groupID == -1)
        return ret;

    WhereConditions where;
    where.equal(this->termGroup, groupID);

    RecList idS = toRecList(select(QStringList() << uid, where));

    for( QSqlRecord &r : idS ) {
        ret << r.value( uid ).toInt();
    }

    return ret;
}

QList<QUuid> NdTbl::getAllNodesUuidsInGroup(QUuid groupUuid)
{
    QList<QUuid> ret;

    WhereConditions where;
    where.equal(this->termGroup, groupUuid.toString());

    RecList nodesRecords = toRecList(select(QStringList() << this->longUID, where));

    for( QSqlRecord &record : nodesRecords ) {
        if(!record.contains( this->longUID ))
            continue;

        QUuid tmpUuid( record.value( this->longUID ).toString() );
        if(!tmpUuid.isNull())
            ret << tmpUuid;
    }

    return ret;
}

QSqlRecord NdTbl::getNode(int id)
{
    WhereConditions where;
    where.equal(uid,id);

    QSqlQuery sel = select(getAllCols(), where);

    if(!sel.next())
        return QSqlRecord();

    return sel.record();
}

QSqlRecord NdTbl::getNode(QUuid uuid)
{
    WhereConditions where;
    where.equal(longUID, uuid.toString());

    QSqlQuery sel = select(getAllCols(), where);

    if(!sel.next())
        return QSqlRecord();

    return sel.record();
}

bool NdTbl::isNodeWithUuidExist(QUuid uuid)
{
    return isUuidExist(uuid);
}

