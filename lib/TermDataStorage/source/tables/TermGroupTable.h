// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QSqlRecord>

#include <CommonTools/GroupUuid.h>
#include <CommonTools/HandyTypes.h>
#include <TermDataInterface/GroupSummary.h>

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
    Result<GroupSummary> group(const GroupUuid& uuid);
    GroupSummary::List   allGroups();

    // Modify
    Result<GroupSummary> addGroup(const GroupSummary& info);
    Result<GroupSummary> updateGroup(const GroupSummary& info);
    Result<GroupSummary> deleteGroup(const GroupUuid& uuid);

private:
    GroupUuid generateNewUuid();

    static GroupSummary createGroupData(const QSqlRecord& rec);
};
