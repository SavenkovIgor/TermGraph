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

#pragma once

#include "tblbase.h"
/*
class EdgTbl: public TblBase
{
public:
    const QString uid     = "uid";
    const QString fromID  = "fromID";
    const QString comment = "comment";
    const QString toID    = "toID";

    EdgTbl(QSqlDatabase *base):
    TblBase("termEdge",base)
    {
        initColumn( uid,        "INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL");
        initColumn( toID,       "INTEGER");
        initColumn( fromID,     "INTEGER");
        initColumn( comment,    "TEXT");
    }

    QList<QPair<int, int> > getAllEdges();
    bool hasThisEdge(int from, int to);

    void clearFromDuplicates();

    void addEdge(int from, int to,QString comment = "");
    void deleteAllEdgesFor (int fromID = -1,int toID = -1);
    void deleteEdge ( int uid );
};
*/
