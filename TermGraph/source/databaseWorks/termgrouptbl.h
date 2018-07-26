#ifndef TERMGROUPTBL_H
#define TERMGROUPTBL_H

#include "tblbase.h"

class TermGroupTbl : public TblBase
{
public:
    static const QString uid;  // TODO: Грохнуть
    const QString longUID = "longUID";
    const QString name    = "name";
    const QString comment = "comment";
    const QString type    = "type";

    TermGroupTbl(QSqlDatabase *base):
        TblBase("termGroup",base)
    {
        initColumn(uid,     "INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL");
        initColumn(longUID, "TEXT");
        initColumn(name,    "TEXT UNIQUE");
        initColumn(comment, "TEXT");
        initColumn(type,    "INTEGER DEFAULT '-1'");
    }

    QList<int>   getAllGroupsUid();
    QList<QUuid> getAllGroupsUuid();
    QStringList getAllGroupsNames(QString area = "", bool withUid = false);

    QSqlRecord getGroup(int id); //TODO: Удалить, когда все данные будут сохранены
    QSqlRecord getGroup(QUuid uuid);

    int getUid(QString   groupName); //TODO: Удалить, когда все данные будут сохранены
    QUuid getUuid(QString groupName);
    bool hasGroupWithName(QString groupName);
    bool hasGroupWithUuid(QUuid uuid);
    bool addGroup(QString name, QString comment, int type);
    bool addGroup(QUuid uuid, QString name, QString comment, int type);

    int getType(QUuid groupUuid);

    void setName(QUuid uuid, QString name);
    void setComment(QUuid uuid, QString comment);
    void setType(QUuid uuid, int type);

    void deleteGroup( QString name );

    bool isUuidExist(QUuid uuid);
    void normalizeUuid();

    QUuid generateNewUuid();
};

#endif // TERMGROUPTBL_H
