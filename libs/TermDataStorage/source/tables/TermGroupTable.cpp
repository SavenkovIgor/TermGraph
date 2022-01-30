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

#include "source/tables/TermGroupTable.h"

#include "source/DbInfo.h"
#include "source/DbTools.h"
#include "source/SqlQueryBuilder.h"

void TermGroupTable::initTable() { DbTools::start(SqlQueryBuilder().createGroupsTable()); }

bool TermGroupTable::exist(const QString &groupName)
{
    auto query = SqlQueryBuilder().selectGroup(groupName);
    DbTools::start(query);

    auto records = DbTools::getAllRecords(std::move(query));

    if (records.empty())
        return false;

    return GroupUuid::create(records.front().value("uuid").toString()).has_value();
}

bool TermGroupTable::exist(const GroupUuid &uuid)
{
    auto query = SqlQueryBuilder().selectOneGroup(uuid);
    DbTools::start(query);

    auto next_exist = query.next();
    assert(next_exist);

    auto count = query.record().value("COUNT( * )").toInt();
    return count > 0;
}

GroupUuid::List TermGroupTable::allUuids()
{
    GroupUuid::List ret;

    auto query = SqlQueryBuilder().selectAllGroupUuids();
    DbTools::start(query);

    auto records = DbTools::getAllRecords(std::move(query));

    for (const auto &record : records) {
        QUuid uuid(record.value("uuid").toString());
        assert(!uuid.isNull());
        if (auto gUuid = GroupUuid::create(uuid))
            ret.push_back(*gUuid);
    }

    return ret;
}

Result<GroupData> TermGroupTable::group(const GroupUuid &uuid)
{
    // If group not exist
    if (!exist(uuid))
        return DbErrorCodes::GroupUuidNotFound;

    auto query = SqlQueryBuilder().selectGroup(uuid);
    DbTools::start(query);
    auto record = DbTools::getRecord(std::move(query));
    return createGroupData(record);
}

GroupData::List TermGroupTable::allGroups()
{
    GroupData::List ret;

    auto query = SqlQueryBuilder().selectAllGroups();
    DbTools::start(query);

    auto records = DbTools::getAllRecords(std::move(query));

    for (auto &record : records) {
        GroupData info = createGroupData(record);
        ret.push_back(std::move(info));
    }

    return ret;
}

Result<GroupData> TermGroupTable::addGroup(const GroupData &info)
{
    GroupData groupInfo = info;

    if (groupInfo.uuid) {
        if (exist(*groupInfo.uuid)) {
            return DbErrorCodes::GroupUuidAlreadyExist;
        }
    } else {
        groupInfo.uuid = generateNewUuid();
    }

    if (groupInfo.name.simplified().isEmpty())
        return DbErrorCodes::GroupNameEmpty;

    if (exist(groupInfo.name))
        return DbErrorCodes::GroupNameAlreadyExist;

    DbTools::start(SqlQueryBuilder().insertGroup(groupInfo));

    return groupInfo;
}

Result<GroupData> TermGroupTable::updateGroup(const GroupData &info)
{
    if (!info.uuid)
        return DbErrorCodes::GroupUuidInvalid;

    if (!exist(*info.uuid))
        return DbErrorCodes::GroupUuidNotFound;

    if (info.name.simplified().isEmpty())
        return DbErrorCodes::GroupNameEmpty;

    for (const auto &group : allGroups()) {
        if (info.name == group.name && info.uuid != group.uuid)
            return DbErrorCodes::GroupNameAlreadyExist;
    }

    DbTools::start(SqlQueryBuilder().updateGroup(info));

    return info;
}

Result<void> TermGroupTable::deleteGroup(const GroupUuid &uuid)
{
    if (!exist(uuid))
        return DbErrorCodes::GroupUuidNotFound;

    DbTools::start(SqlQueryBuilder().deleteGroup(uuid));
    return outcome::success();
}

GroupUuid TermGroupTable::generateNewUuid()
{
    while (true) {
        auto uuid = GroupUuid::generate();
        if (!exist(uuid))
            return uuid;
    }
}

GroupData TermGroupTable::createGroupData(const QSqlRecord &rec)
{
    GroupData info;

    info.uuid    = GroupUuid::create(rec.value(GroupJsonValidator::uuidKey).toString());
    info.name    = rec.value(GroupJsonValidator::nameKey).toString();
    info.comment = rec.value(GroupJsonValidator::commentKey).toString();

    return info;
}
