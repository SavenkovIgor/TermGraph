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

    // Adding, deleting
    bool addGroup(QString name, QString comment, int type);
    bool addGroup(QUuid uuid, QString name, QString comment, int type);
    void deleteGroup(QString name);

    // Getters
    QSqlRecord getGroup(QUuid uuid);
    QString getName(QUuid uuid);
    int getType(QUuid groupUuid);
    QUuid getUuid(QString groupName);

    // Massive getters
    QList<QUuid> getAllGroupsUuid();
    QStringList getAllGroupsNames(QString area = "", bool withUid = false);

    // Setters
    void setName(QUuid uuid, QString name);
    void setComment(QUuid uuid, QString comment);
    void setType(QUuid uuid, int type);

    // Checkers
    bool isUuidExist(QUuid uuid);
    bool hasGroupWithName(QString groupName);

private:
    QUuid generateNewUuid();
};

#endif // TERMGROUPTBL_H
