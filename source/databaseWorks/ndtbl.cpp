#include "ndtbl.h"

QString NdTbl::addNode(QString name)
{
    //По UUID!!
    QUuid uuid;

    for( int i = 0; i < 1000; i++ ) {
        uuid = QUuid::createUuid();
        if(!isUuidExist(uuid.toString())){
            if( i == 0)
                qDebug()<<"break at 0";
            break;
        }
    }

    if( name.simplified() == "" )
        return ""; //Создать вершину не удалось

    QList<InsertContainer> values;
    values << InsertContainer(this->term,name);
    values << InsertContainer(this->longUID,uuid.toString());

    insertInto(values);

    WhereConditions where;
    where.equal(this->term,name);
    where.equal(this->longUID,uuid.toString());
    QSqlQuery sel = select(QStringList() << this->uid, where);

    if(!sel.next())
        return "";

    updateLastEdit(uuid.toString());
    return uuid.toString();
}

int NdTbl::getRemindNum(int uid)
{
    QSqlRecord rec = getNode(uid);

    if( rec.isEmpty() )
        return -1;
    return rec.value(this->remindNum).toInt();
}

void NdTbl::setName(QString uuid, QString name)
{
    setField(this->term,uuid,name);
    updateLastEdit(uuid);
}

void NdTbl::setWordForms(QString uuid, QString forms)
{
    setField(this->termForms,uuid,forms);
    updateLastEdit(uuid);
}

void NdTbl::setDefinition(QString uuid, QString def)
{
    setField(this->definition,uuid,def);
    updateLastEdit(uuid);
}

void NdTbl::setDescription(QString uuid, QString desc)
{
    setField(this->description,uuid,desc);
    updateLastEdit(uuid);
}

void NdTbl::setExamples(QString uuid, QString examp)
{
    setField(this->examples,uuid,examp);
    updateLastEdit(uuid);
}

void NdTbl::setWikiRef(QString uuid, QString wRef)
{
    setField(this->wikiRef,uuid,wRef);
    updateLastEdit(uuid);
}

void NdTbl::setWikiImg(QString uuid, QString wImg)
{
    setField(this->wikiImg,uuid,wImg);
    updateLastEdit(uuid);
}

void NdTbl::setGroup(QString uuid, int group)
{
    setField(this->termGroup, uuid, QString::number(group));
    updateLastEdit(uuid);
}

void NdTbl::setGroup(QUuid nodeUuid, QUuid groupUuid)
{
    setField(this->termGroup, nodeUuid.toString(), groupUuid.toString());
    updateLastEdit(nodeUuid.toString());
}

void NdTbl::setAtLearn(QString uuid, bool learn)
{
    if( learn )
        setField(this->atLearn,uuid,"1");
    else
        setField(this->atLearn,uuid,"0");
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

void NdTbl::updateLastEdit(int uid)
{
    setField(this->lastEdit,uid,QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
}

void NdTbl::updateLastEdit(QString uuid)
{
    setField(this->lastEdit,uuid,QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
}

bool NdTbl::isUuidExist(QString longUuid)
{
    WhereConditions where;
    where.equal(this->longUID,longUuid);

    RecList recs = toRecList(select(QStringList() << this->longUID, where));
    return !recs.isEmpty();
}

QList<int> NdTbl::getAllNodesUid()
{
    QList<int> ret;

    RecList idS = toRecList(select(QStringList() << this->uid));
    for( QSqlRecord &r : idS ) {
        ret << r.value( this->uid ).toInt();
    }

    qDebug()<<ret;
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

QSqlRecord NdTbl::getNode(int id)
{
    WhereConditions where;
    where.equal(uid,id);

    QSqlQuery sel = select(getAllCols(), where);

    if(!sel.next())
        return QSqlRecord();

    return sel.record();
}

bool NdTbl::isNodeWithUuidExist(QString uuid)
{
    return isUuidExist(uuid);
}

