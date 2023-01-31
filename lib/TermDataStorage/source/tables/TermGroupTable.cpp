// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/tables/TermGroupTable.h"

#include "source/DbTools.h"
#include "source/SqlQueryBuilder.h"

void TermGroupTable::initTable() { DbTools::start(SqlQueryBuilder().createGroupsTable()); }

bool TermGroupTable::exist(const QString &groupName)
{
    auto query = SqlQueryBuilder().selectGroup(groupName);
    DbTools::start(query);

    auto records = DbTools::getAllRecords(std::move(query));

    if (records.empty()) {
        return false;
    }

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

Result<GroupData> TermGroupTable::group(const GroupUuid &uuid)
{
    // If group not exist
    if (!exist(uuid)) {
        return ErrorCodes::GroupUuidNotFound;
    }

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
            return ErrorCodes::GroupUuidAlreadyExist;
        }
    } else {
        groupInfo.uuid = generateNewUuid();
    }

    if (groupInfo.name.simplified().isEmpty()) {
        return ErrorCodes::GroupNameEmpty;
    }

    if (exist(groupInfo.name)) {
        return ErrorCodes::GroupNameAlreadyExist;
    }

    DbTools::start(SqlQueryBuilder().insertGroup(groupInfo));

    return groupInfo;
}

Result<GroupData> TermGroupTable::updateGroup(const GroupData &info)
{
    if (!info.uuid) {
        return ErrorCodes::GroupUuidInvalid;
    }

    if (!exist(*info.uuid)) {
        return ErrorCodes::GroupUuidNotFound;
    }

    if (info.name.simplified().isEmpty()) {
        return ErrorCodes::GroupNameEmpty;
    }

    for (const auto &group : allGroups()) {
        if (info.name == group.name && info.uuid != group.uuid) {
            return ErrorCodes::GroupNameAlreadyExist;
        }
    }

    DbTools::start(SqlQueryBuilder().updateGroup(info));

    return info;
}

Result<GroupData> TermGroupTable::deleteGroup(const GroupUuid &uuid)
{
    if (auto groupData = group(uuid)) {
        DbTools::start(SqlQueryBuilder().deleteGroup(uuid));
        return groupData;
    }

    return ErrorCodes::GroupUuidNotFound;
}

GroupUuid TermGroupTable::generateNewUuid()
{
    while (true) {
        auto uuid = GroupUuid::generate();
        if (!exist(uuid)) {
            return uuid;
        }
    }
}

GroupData TermGroupTable::createGroupData(const QSqlRecord &rec)
{
    GroupData info;

    info.uuid    = GroupUuid::create(rec.value(JsonTools::uuidKey).toString());
    info.name    = rec.value(JsonTools::nameKey).toString();
    info.comment = rec.value(JsonTools::commentKey).toString();
    info.size    = rec.value(JsonTools::sizeKey).toInt();

    return info;
}
