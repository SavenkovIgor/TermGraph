#ifndef TERMGROUPTBL_H
#define TERMGROUPTBL_H

#include "./tblbase.h"
#include "./dbtablenames.h"
#include "./columns/tcolumn.h"
#include "../Model/TerminGroup/groupinfocontainer.h"

class TermGroupTable : public TblBase
{
public:
    TermGroupTable(QSqlDatabase* base)
        : TblBase(TableName::GROUPS, base)
    {}
    ~TermGroupTable() override = default;

    // Add, update, delete
    bool addGroup(const GroupInfoContainer& info);
    bool updateGroup(const GroupInfoContainer& info);
    void deleteGroup(const QUuid& uuid);

    // Getters
    GroupInfoContainer getGroup(const QUuid& uuid);
    RecVector getAllUuidsAndNames();
    UuidList getAllUuids();

    QUuid getUuid(const QString& groupName) const;

    // Checkers
    bool hasGroupWithUuid(const QUuid& uuid);
    bool hasGroupWithName(const QString& groupName);

    void initTable() override;

protected:
    TColumn::List getAllColumns() const override;

private:
    QUuid generateNewUuid();
};

#endif  // TERMGROUPTBL_H
