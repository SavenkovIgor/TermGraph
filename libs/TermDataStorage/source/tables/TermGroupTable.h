/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

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
    // TODO: Return object after add, update, delete
    Result<GroupData> addGroup(const GroupData& info);
    Result<GroupData> updateGroup(const GroupData& info);
    Result<void>      deleteGroup(const GroupUuid& uuid);

private:
    GroupUuid generateNewUuid();

    static GroupData createGroupData(const QSqlRecord& rec);
};
