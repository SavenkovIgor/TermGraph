#include "ndtbl.h"

int NdTbl::addNode(QString name)
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
        return -1; //Создать вершину не удалось

    QStringList cols;
    cols << this->term;
    cols << this->longUID;
    QStringList vals;
    vals << name;
    vals << uuid.toString();

    insertInto(cols,vals);

    SqCond where;
    where[this->term] = name;
    where[this->longUID] = uuid.toString();
    QSqlQuery sel = select(QStringList()<<this->uid,where);

    if(!sel.next())
        return -1;

    int id = sel.record().value(this->uid).toInt();
    updateLastEdit(id);
    return id;
}

int NdTbl::getRemindNum(int uid)
{
    QSqlRecord rec = getNode(uid);

    if( rec.isEmpty() )
        return -1;
    return rec.value(this->remindNum).toInt();
}

void NdTbl::setName(int uid, QString name)
{
    setField(this->term,uid,name);
    updateLastEdit(uid);
}

void NdTbl::setTForms(int uid, QString forms)
{
    setField(this->termForms,uid,forms);
    updateLastEdit(uid);
}

void NdTbl::setDefinition(int uid, QString def)
{
    setField(this->definition,uid,def);
    updateLastEdit(uid);
}

void NdTbl::setDescription(int uid, QString desc)
{
    setField(this->description,uid,desc);
    updateLastEdit(uid);
}

void NdTbl::setExamples(int uid, QString examp)
{
    setField(this->examples,uid,examp);
    updateLastEdit(uid);
}

void NdTbl::setWikiRef(int uid, QString wRef)
{
    setField(this->wikiRef,uid,wRef);
    updateLastEdit(uid);
}

void NdTbl::setWikiImg(int uid, QString wImg)
{
    setField(this->wikiImg,uid,wImg);
    updateLastEdit(uid);
}

void NdTbl::setIsRoot(int uid, bool isRoot)
{
    if( isRoot )
        setField(this->isRoot,uid,"1");
    else
        setField(this->isRoot,uid,"0");
    updateLastEdit(uid);
}

void NdTbl::setGroup(int uid, int group)
{
    setField(this->termGroup,uid,QString::number(group));
    updateLastEdit(uid);
}

void NdTbl::setAtLearn(int uid, bool learn)
{
    if( learn )
        setField(this->atLearn,uid,"1");
    else
        setField(this->atLearn,uid,"0");
}

//void NdTbl::setRemindToday(int uid)
//{
//    setRemindNum(uid, getRemindNum(uid) + 1, QDateTime::currentDateTime() );
//}

void NdTbl::setRemindNum(int uid, int num, QDate date)
{
    SqCond set;
    set[this->remindNum] = QString::number(num);
    set[this->lastRemind] = date.toString();

    SqCond where;
    where[this->uid] = QString::number(uid);
    updateWhere(set,where);
}

void NdTbl::deleteNode(int id)
{
    deleteRecord(id);
}

void NdTbl::normalizeUuid()
{
    QList<int> ndLst = getAllNodesList();
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

bool NdTbl::isUuidExist(QString longUuid)
{
    SqCond where;
    where[this->longUID] = longUuid;

    RecList recs = toRecList( select(QStringList()<<this->longUID,where) );
    return !recs.isEmpty();
}

QList<int> NdTbl::getAllNodesList()
{
    QList<int> ret;

    RecList idS = toRecList( select( QStringList() << this->uid ) );
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

    SqCond where;
    where[this->termGroup] = QString::number( groupID );

    RecList idS = toRecList(select( QStringList()<<uid, where ));

    for( QSqlRecord &r : idS ) {
        ret << r.value( uid ).toInt();
    }

    return ret;
}

QSqlRecord NdTbl::getNode(int id)
{
    QSqlQuery sel = select("*",uid + " = " + vv(QString::number(id)));

    if(!sel.next())
        return QSqlRecord();

    return sel.record();
}

