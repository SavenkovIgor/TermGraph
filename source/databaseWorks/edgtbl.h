#ifndef EDGTBL_H
#define EDGTBL_H

#include "tblbase.h"

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
        addCol( uid,        "INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL");
        addCol( toID,       "INTEGER");
        addCol( fromID,     "INTEGER");
        addCol( comment,    "TEXT");
    }

    QList<QPair<int, int> > getAllEdges();
    bool hasThisEdge(int from, int to);

    void clearFromDuplicates();

    void addEdge(int from, int to,QString comment = "");
    void deleteAllEdgesFor (int fromID = -1,int toID = -1);
    void deleteEdge ( int uid );
};

#endif // EDGTBL_H
