// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QSqlRecord>

#include <CommonTools/GroupUuid.h>
#include <CommonTools/HandyTypes.h>
#include <TermDataInterface/DataStorageErrors.h>
#include <TermDataInterface/GroupData.h>

// TODO: GroupUuid to GroupData
class TermGroupTable
{
public:
    TermGroupTable()  = default;
    ~TermGroupTable() = default;

    // Table stuff
    void initTable();

    // Checkers
    bool exist(const QString& groupName);
    bool exist(const GroupUuid& uuid);

    // Getters
    GroupUuid::List   allUuids();
    Result<GroupData> group(const GroupUuid& uuid);
    GroupData::List   allGroups();

    // Modify
    Result<GroupData> addGroup(const GroupData& info);
    Result<GroupData> updateGroup(const GroupData& info);
    Result<GroupData> deleteGroup(const GroupUuid& uuid);

private:
    GroupUuid generateNewUuid();

    static GroupData createGroupData(const QSqlRecord& rec);
};
