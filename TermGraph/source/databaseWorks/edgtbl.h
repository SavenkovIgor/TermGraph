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
