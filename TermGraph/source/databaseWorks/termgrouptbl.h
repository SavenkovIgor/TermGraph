#ifndef TERMGROUPTBL_H
#define TERMGROUPTBL_H

#include "./tblbase.h"

class TermGroupColumn {
public:
    // TODO: Грохнуть короткий uid
    constexpr static auto uid     = TColumn("uid",     "INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL");
    constexpr static auto longUID = TColumn("longUID", "TEXT");
    constexpr static auto name    = TColumn("name",    "TEXT UNIQUE");
    constexpr static auto comment = TColumn("comment", "TEXT");
    constexpr static auto type    = TColumn("type",    "INTEGER DEFAULT '-1'");

    constexpr static TColumn columns[] = {
        uid,
        longUID,
        name,
        comment,
        type
    };
};

class TermGroupTable : public TblBase
{
public:
    TermGroupTable(QSqlDatabase* base):
        TblBase("termGroup", base)
    {
        initColumn(TermGroupColumn::uid);
        initColumn(TermGroupColumn::longUID);
        initColumn(TermGroupColumn::name);
        initColumn(TermGroupColumn::comment);
        initColumn(TermGroupColumn::type);
    }

    // Adding, deleting
    bool addGroup(QString name, QString comment, int type);
    bool addGroup(QUuid uuid, QString name, QString comment, int type);
    void deleteGroup(QUuid uuid);

    // Getters
    QSqlRecord getGroup(const QUuid &uuid);
    QString getName(const QUuid &uuid) const;
    int getType(QUuid groupUuid) const;
    QUuid getUuid(const QString &groupName) const;

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
