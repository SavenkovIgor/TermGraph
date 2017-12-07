#ifndef TERMGROUPTBL_H
#define TERMGROUPTBL_H

#include "tblbase.h"

class TermGroupTbl : public TblBase
{
public:
    const QString uid     = "uid";
    const QString name    = "name";
    const QString comment = "comment";
    const QString type    = "type";

    TermGroupTbl(QSqlDatabase *base):
        TblBase("termGroup",base)
    {
        addCol(uid,     "INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL");
        addCol(name,    "TEXT UNIQUE");
        addCol(comment, "TEXT");
        addCol(type,    "INTEGER DEFAULT '-1'");
    }

    bool addGroup( QString name, QString comment, int type );
    QStringList getAllGroupsList(QString area = "", bool withUid = false);

    int     getUid  ( QString   groupName );
    QString getName ( int       groupUid );

    int     getType ( int groupUid );

    void deleteGroup( QString name );
};

#endif // TERMGROUPTBL_H
