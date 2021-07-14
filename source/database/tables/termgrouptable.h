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

#include <handytypes.h>
#include <libs/datainterface/groupdata.h>

class TermGroupTable
{
public:
    TermGroupTable()  = default;
    ~TermGroupTable() = default;

    // Add, update, delete
    bool addGroup(const GroupData& info);
    bool updateGroup(const GroupData& info);
    void deleteGroup(const QUuid& groupUuid);

    // Getters
    UuidList        getAllUuids();
    GroupData       getGroup(const QUuid& uuid);
    GroupData::List getGroups();

    QUuid getUuid(const QString& groupName) const;

    // Checkers
    bool groupExist(const QUuid& uuid);
    bool groupWithNameExist(const QString& groupName);

    void initTable();

private:
    QUuid generateNewUuid();

    static GroupData sqlRecordToGroupInfo(const QSqlRecord& rec);
};