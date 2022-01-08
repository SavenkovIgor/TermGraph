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

Result<void> TermGroupTable::addGroup(const GroupData& info)
{
    GroupData groupInfo = info;

    if (groupInfo.uuid.isNull()) {
        groupInfo.uuid = generateNewUuid();
    } else {
        if (groupExist(groupInfo.uuid))
            return DbErrorCodes::UuidAlreadyExist;
    }

    if (groupInfo.name.simplified().isEmpty())
        return DbErrorCodes::GroupNameEmpty;

    if (groupWithNameExist(groupInfo.name))
        return DbErrorCodes::GroupNameAlreadyExist;

    DbTools::start(SqlQueryBuilder().insertGroup(groupInfo));

    return outcome::success();
}

Result<void> TermGroupTable::updateGroup(const GroupData& info)
{
    if (info.uuid.isNull())
        return DbErrorCodes::UuidEmpty;

    if (!groupExist(info.uuid))
        return DbErrorCodes::UuidNotFound;

    if (info.name.simplified().isEmpty())
        return DbErrorCodes::GroupNameEmpty;

    for (const auto& group : getGroups()) {
        if (info.name == group.name && info.uuid != group.uuid)
            return DbErrorCodes::GroupNameAlreadyExist;
    }

    DbTools::start(SqlQueryBuilder().updateGroup(info));

    return outcome::success();
}

GroupUuid::List TermGroupTable::getAllUuids()
{
    GroupUuid::List ret;

    auto query = SqlQueryBuilder().selectAllGroupUuids();
    DbTools::start(query);

    auto records = DbTools::getAllRecords(std::move(query));

    for (const auto& record : records) {
        QUuid uuid(record.value("uuid").toString());
        assert(!uuid.isNull());
        if (auto gUuid = GroupUuid::create(uuid))
            ret.push_back(*gUuid);
    }

    return ret;
}

Result<void> TermGroupTable::deleteGroup(const GroupUuid& uuid)
{
    if (!groupExist(uuid))
        return DbErrorCodes::UuidNotFound;

    DbTools::start(SqlQueryBuilder().deleteGroup(uuid));
    return outcome::success();
}

bool TermGroupTable::groupExist(const QUuid& uuid)
{
    if (auto gUuid = GroupUuid::create(uuid))
        return groupExist(*gUuid);

    return false;
}

bool TermGroupTable::groupExist(const GroupUuid& uuid)
{
    auto query = SqlQueryBuilder().selectOneGroup(uuid);
    DbTools::start(query);

    auto next_exist = query.next();
    assert(next_exist);

    auto count = query.record().value("COUNT( * )").toInt();
    return count > 0;
}

QUuid TermGroupTable::generateNewUuid()
{
    QUuid uuid;
    while (true) {
        uuid = QUuid::createUuid();
        if (!groupExist(uuid))
            return uuid;
    }
}

QUuid TermGroupTable::getUuid(const QString& groupName) const
{
    auto query = SqlQueryBuilder().selectGroup(groupName);
    DbTools::start(query);

    auto records = DbTools::getAllRecords(std::move(query));

    if (records.empty())
        return QUuid();

    return QUuid(records.front().value("uuid").toString());
}

bool TermGroupTable::groupWithNameExist(const QString& groupName) { return !getUuid(groupName).isNull(); }

void TermGroupTable::initTable() { DbTools::start(SqlQueryBuilder().createGroupsTable()); }

Result<GroupData> TermGroupTable::getGroup(const GroupUuid& uuid)
{
    // If group not exist
    if (!groupExist(uuid))
        return DbErrorCodes::UuidNotFound;

    auto query = SqlQueryBuilder().selectGroup(uuid);
    DbTools::start(query);
    auto record = DbTools::getRecord(std::move(query));
    return sqlRecordToGroupInfo(record);
}

GroupData::List TermGroupTable::getGroups()
{
    GroupData::List ret;

    auto query = SqlQueryBuilder().selectAllGroups();
    DbTools::start(query);

    auto records = DbTools::getAllRecords(std::move(query));

    for (auto& record : records) {
        GroupData info = sqlRecordToGroupInfo(record);
        ret.push_back(std::move(info));
    }

    return ret;
}

GroupData TermGroupTable::sqlRecordToGroupInfo(const QSqlRecord& rec)
{
    GroupData info;

    info.uuid    = QUuid(rec.value(GroupJsonValidator::uuidKey).toString());
    info.name    = rec.value(GroupJsonValidator::nameKey).toString();
    info.comment = rec.value(GroupJsonValidator::commentKey).toString();

    return info;
}
