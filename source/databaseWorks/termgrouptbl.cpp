#include "termgrouptbl.h"


bool TermGroupTbl::addGroup(QString name, QString comment, int type)
{
    if(name.simplified() == "")
        return false;

    QStringList cols;
    cols << this->name;
    cols << this->comment;
    cols << this->type;

    QStringList vals;
    vals << name;
    vals << comment;
    vals << QString::number(type);

    return insertInto(cols,vals);
}

QList<int> TermGroupTbl::getAllGroupsUid()
{
    QList<int> ret;

    QSqlQuery q = select(QStringList()<<this->uid,"","ORDER BY uid");

    for(int i=0;i<1000000;i++) {
        if(!q.next())
            break;

        ret << q.record().value(this->uid).toInt();
    }

    return ret;
}

void TermGroupTbl::deleteGroup(QString name)
{
    deleteWhere( this->name + " = " + vv(name) );
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
    SqCond where;
    where[this->name] = groupName;

    QSqlQuery q = select(QStringList()<<this->uid,where);
    if(!q.next())
        return -1;

    return q.record().value(this->uid).toInt();
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

QStringList TermGroupTbl::getAllGroupsNames(QString area, bool withUid)
{
    QStringList ret;

    QSqlQuery q = select(QStringList()<<this->name<<this->uid,"","ORDER BY uid");

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
    QSqlQuery sel = select("*",this->uid + " = " + vv(QString::number(id)));

    if(!sel.next())
        return QSqlRecord();

    return sel.record();
}
