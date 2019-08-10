#pragma once

#include "source/databaseWorks/tblbase.h"
#include "source/databaseWorks/dbtablenames.h"
#include "source/databaseWorks/columns/tcolumn.h"
#include "source/Model/TerminGroup/groupinfocontainer.h"

class TermGroupTable : public TblBase
{
public:
    TermGroupTable(QSqlDatabase* base)
        : TblBase(base)
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

    const char* tableName() const override;
    TColumn primaryKey() const override;
    void initTable() override;

protected:
    TColumn::List getAllColumns() const override;

private:
    QUuid generateNewUuid();
};
