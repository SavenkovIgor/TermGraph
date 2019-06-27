#ifndef TERMGROUPTBL_H
#define TERMGROUPTBL_H

#include "./tblbase.h"
#include "./dbtablenames.h"
#include "./columns/tcolumn.h"
#include "../Model/TerminGroup/groupinfocontainer.h"

class TermGroupTable : public TblBase
{
public:
    TermGroupTable(QSqlDatabase* base): TblBase(TableName::GROUPS, base) { }
    ~TermGroupTable() override = default;

    // Adding, deleting
    bool addGroup(QString name, QString comment, int type);
    bool addGroup(QUuid uuid, QString name, QString comment, int type);
    bool addGroup(const GroupInfoContainer& info);
    void deleteGroup(const QUuid& uuid);

    // Getters
    GroupInfoContainer getGroupInfoContainer(const QUuid& uuid);
    QUuid getUuid(const QString &groupName) const;
    RecVector getAllUuidsAndNames();

    // Massive getters
    UuidList getAllUuids();

    // Setters
    void setName(QUuid uuid, QString name);
    void setComment(QUuid uuid, QString comment);
    void setType(QUuid uuid, int type);

    // Checkers
    bool hasGroupWithUuid(const QUuid &uuid);
    bool hasGroupWithName(QString groupName);

    void initTable() override;

protected:
    TColumn::List getAllColumns() const override;

private:
    QUuid generateNewUuid();
};

#endif  // TERMGROUPTBL_H
