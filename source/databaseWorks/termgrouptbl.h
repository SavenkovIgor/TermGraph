#ifndef TERMGROUPTBL_H
#define TERMGROUPTBL_H

#include "tblbase.h"

class TermGroupTbl : public TblBase
{
public:
    const QString uid     = "uid";
    const QString longUID = "longUID";
    const QString name    = "name";
    const QString comment = "comment";
    const QString type    = "type";

    TermGroupTbl(QSqlDatabase *base):
        TblBase("termGroup",base)
    {
        addCol(uid,     "INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL");
        addCol(longUID, "TEXT");
        addCol(name,    "TEXT UNIQUE");
        addCol(comment, "TEXT");
        addCol(type,    "INTEGER DEFAULT '-1'");
    }

    QList<int>  getAllGroupsUid();
    QStringList getAllGroupsNames(QString area = "", bool withUid = false);

    QSqlRecord getGroup(int id);

    int     getUid  ( QString   groupName );
    bool addGroup( QString name, QString comment, int type );

    QString getName    ( int groupUid );
    QString getLongUid ( int groupUid );
    int     getType    ( int groupUid );

    void deleteGroup( QString name );

    void normalizeUuid();
};

#endif // TERMGROUPTBL_H
