#ifndef TERMGROUPTBL_H
#define TERMGROUPTBL_H

#include "./tblbase.h"

class TermGroupColumn {
public:
    constexpr static auto uid     = "uid";  // TODO: Грохнуть
    constexpr static auto longUID = "longUID";
    constexpr static auto name    = "name";
    constexpr static auto comment = "comment";
    constexpr static auto type    = "type";
};

class TermGroupTable : public TblBase
{
public:
    TermGroupTable(QSqlDatabase* base):
        TblBase("termGroup", base)
    {
        initColumn(TColumn(TermGroupColumn::uid,     "INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL"));
        initColumn(TColumn(TermGroupColumn::longUID, "TEXT"));
        initColumn(TColumn(TermGroupColumn::name,    "TEXT UNIQUE"));
        initColumn(TColumn(TermGroupColumn::comment, "TEXT"));
        initColumn(TColumn(TermGroupColumn::type,    "INTEGER DEFAULT '-1'"));
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
