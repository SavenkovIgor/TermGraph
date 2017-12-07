#include "edgtbl.h"


QList<QPair<int, int> > EdgTbl::getAllEdges()
{
    QList< QPair<int, int> > ret;
    QPair<int, int> p;
    QSqlQuery q = select(getAllCols());

    for(int i = 0; i < 10000000; i++){
        if(!q.next())
            break;
        int from = q.record().value(fromID).toInt();
        int to   = q.record().value(toID).toInt();

        p.first = from;
        p.second = to;

        ret << p;
    }

    return ret;
}

bool EdgTbl::hasThisEdge(int from, int to)
{
    QList<int> ret;

    SqCond where;
    where[this->fromID] = QString::number( from );
    where[this->toID]   = QString::number( to );

    RecList idS = toRecList( select( QStringList()<<uid, where ) );

    return !idS.isEmpty(); //Если список пуст - такого ребра нет
}

void EdgTbl::clearFromDuplicates()
{
    QSqlQuery q = select( QStringList() << this->uid << this->toID << this->fromID);

    for(int i = 0; i < 10000000; i++){
        if(!q.next())
            break;
        int uid = q.record().value(this->uid).toInt();
//        qDebug() << "uid" << uid;

        SqCond where;
        where[this->toID]   = q.record().value( this->toID   ).toString();
        where[this->fromID] = q.record().value( this->fromID ).toString();

        QSqlQuery tmpQry = select( QStringList() << this->uid, where );
        QStringList forDel;
        bool first = true;

        for( int i=0; i < 10000000; i++ ) {
            if( !tmpQry.next() )
                break;
            if( first ) { //Первый пропускаем
                first = false;
//                qDebug()<<"first"<<tmpQry.record().value( this->uid ).toString();
            } else {
                forDel << tmpQry.record().value( this->uid ).toString();
            }
        }

        for( QString s : forDel ) {
//            qDebug()<<"deleting"<<s;
            deleteEdge( s.toInt() );
        }

    }
}

void EdgTbl::addEdge(int from, int to, QString comment)
{
    bool edgeDuplicate = hasThisEdge( from, to ) || hasThisEdge( to, from );
    if( edgeDuplicate ) {
        qDebug()<<"edgeDuplicate";
        return;
    }

    QStringList cols;
    cols << fromID;
    cols << toID;
    cols << this->comment;

    QStringList vals;
    vals << QString::number(from);
    vals << QString::number(to);
    vals << comment;

    insertInto(cols,vals);
}

void EdgTbl::deleteAllEdgesFor(int fromID, int toID)
{
    QString from = vv(QString::number(fromID));
    QString to   = vv(QString::number(toID));

    deleteWhere(this->toID + " = " + to + " OR " + this->fromID + " = " + from);
}

void EdgTbl::deleteEdge (int uid)
{
    QString uidStr = vv(QString::number(uid));

    deleteWhere( this->uid + " = " + uidStr );
}
