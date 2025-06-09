// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QSqlRecord>

#include "source/CommonTools/GroupUuid.h"
#include "source/CommonTools/HandyTypes.h"
#include "source/TermDataInterface/GroupSummary.h"

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
    Expected<GroupSummary> group(const GroupUuid& uuid);
    GroupSummary::List     allGroups();

    // Modify
    Expected<GroupSummary> addGroup(const GroupSummary& info);
    Expected<GroupSummary> updateGroup(const GroupSummary& info);
    Expected<GroupSummary> deleteGroup(const GroupUuid& uuid);

private:
    GroupUuid generateNewUuid();

    static GroupSummary createGroupSummary(const QSqlRecord& rec);
};
