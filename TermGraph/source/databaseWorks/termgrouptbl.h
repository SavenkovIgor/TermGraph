#ifndef TERMGROUPTBL_H
#define TERMGROUPTBL_H

#include "./tblbase.h"

class TermGroupColumn {
public:
    static const QString uid;  // TODO: Грохнуть
    static const QString longUID;
    static const QString name;
    static const QString comment;
    static const QString type;
};

class TermGroupTable : public TblBase
{
public:
    TermGroupTable(QSqlDatabase* base):
        TblBase("termGroup", base)
    {
        initColumn(TermGroupColumn::uid,     "INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL");
        initColumn(TermGroupColumn::longUID, "TEXT");
        initColumn(TermGroupColumn::name,    "TEXT UNIQUE");
        initColumn(TermGroupColumn::comment, "TEXT");
        initColumn(TermGroupColumn::type,    "INTEGER DEFAULT '-1'");
    }

    // Adding, deleting
    bool addGroup(QString name, QString comment, int type);
    bool addGroup(QUuid uuid, QString name, QString comment, int type);
    void deleteGroup(QString name);

    // Getters
    QSqlRecord getGroup(const QUuid &uuid);
    QString getName(const QUuid &uuid);
    int getType(QUuid groupUuid);
    QUuid getUuid(const QString &groupName);

    // Massive getters
    QList<QUuid> getAllGroupsUuid();

    // Setters
    void setName(QUuid uuid, QString name);
    void setComment(QUuid uuid, QString comment);
    void setType(QUuid uuid, int type);

    // Checkers
    bool hasGroupWithUuid(const QUuid &uuid);
    bool hasGroupWithName(QString groupName);

private:
    QUuid generateNewUuid();
};

#endif  // TERMGROUPTBL_H
