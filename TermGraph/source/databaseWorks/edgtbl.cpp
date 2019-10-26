/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

#include "edgtbl.h"
/*

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
    WhereCondition where;
    where.equal(this->fromID,from);
    where.equal(this->toID,to);

    auto idS = toRecVector(select(QStringList()<< uid, where));

    return !idS.isEmpty(); //Если список пуст - такого ребра нет
}

void EdgTbl::clearFromDuplicates()
{
    QSqlQuery q = select(QStringList() << this->uid << this->toID << this->fromID);

    for(int i = 0; i < 10000000; i++){
        if(!q.next())
            break;
//        int uid = q.record().value(this->uid).toInt();
//        qDebug() << "uid" << uid;

        WhereCondition where;
        where.equal(this->toID,  q.record().value( this->toID  ).toString());
        where.equal(this->fromID,q.record().value( this->fromID).toString());

        QSqlQuery tmpQry = select(QStringList() << this->uid, where);
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

    QList<InsertContainer> values;
    values << InsertContainer(this->fromID,from);
    values << InsertContainer(this->toID,to);
    values << InsertContainer(this->comment,comment);

    insertInto(values);
}

void EdgTbl::deleteAllEdgesFor(int fromID, int toID)
{
    WhereCondition where(Or);
    where.equal(this->toID,toID);
    where.equal(this->fromID,fromID);

    deleteWhere(where);
}

void EdgTbl::deleteEdge (int uid)
{
    WhereCondition where;
    where.equal(this->uid,uid);
    deleteWhere(where);
}
*/
