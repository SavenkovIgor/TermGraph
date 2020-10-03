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

#include "source/Database/tables/termgrouptable.h"

#include "source/Database/dbinfo.h"
#include "source/Database/dbtools.h"
#include "source/Database/sqlquerybuilder.h"

bool TermGroupTable::addGroup(const GroupInfoContainer& info)
{
    GroupInfoContainer groupInfo = info;

    if (groupInfo.uuid.isNull())
        groupInfo.uuid = generateNewUuid();

    if (groupInfo.name.simplified().isEmpty())
        return false;

    if (groupWithNameExist(groupInfo.name))
        return false;

    auto query = SqlQueryBuilder().insertGroup(groupInfo);
    return DbTools::startQuery2(query);
}

bool TermGroupTable::updateGroup(const GroupInfoContainer& info)
{
    if (info.uuid.isNull())
        return false;

    if (!groupExist(info.uuid))
        return false;

    auto query = SqlQueryBuilder().updateGroup(info);
    DbTools::startQuery2(query);

    return true;
}

UuidList TermGroupTable::getAllUuids()
{
    UuidList ret;

    auto query = SqlQueryBuilder().selectAllGroupUuids();
    DbTools::startQuery2(query);

    auto records = DbTools::getAllRecords(std::move(query));

    for (const auto& record : records) {
        QUuid uuid(record.value("uuid").toString());
        assert(!uuid.isNull());
        ret.push_back(uuid);
    }

    return ret;
}

void TermGroupTable::deleteGroup(const QUuid& groupUuid)
{
    auto query = SqlQueryBuilder().deleteGroup(groupUuid);
    DbTools::startQuery2(query);
}

bool TermGroupTable::groupExist(const QUuid& uuid)
{
    auto query = SqlQueryBuilder().selectOneGroup(uuid);
    DbTools::startQuery2(query);

    if (!query.next())
        return false;

    auto count = query.record().value("COUNT( * )").toInt();
    return count > 0;
}

QUuid TermGroupTable::generateNewUuid()
{
    QUuid uuid;
    for (int i = 0; i < 1000; i++) {
        uuid = QUuid::createUuid();
        if (!groupExist(uuid)) {
            break;
        }
    }
    return uuid;
}

QUuid TermGroupTable::getUuid(const QString& groupName) const
{
    auto query = SqlQueryBuilder().selectGroup(groupName);
    DbTools::startQuery2(query);

    auto records = DbTools::getAllRecords(std::move(query));

    if (records.isEmpty())
        return QUuid();

    return QUuid(records.first().value("uuid").toString());
}

bool TermGroupTable::groupWithNameExist(const QString& groupName) { return !getUuid(groupName).isNull(); }

void TermGroupTable::initTable()
{
    auto query = SqlQueryBuilder().createGroupsTable();
    DbTools::startQuery2(query);
}

GroupInfoContainer TermGroupTable::getGroup(const QUuid& uuid)
{
    auto query = SqlQueryBuilder(DbConnectionName::threadLoadingConnection).selectGroup(uuid);
    DbTools::startQuery2(query);
    auto record = DbTools::getRecord(std::move(query));
    return sqlRecordToGroupInfo(record);
}

GroupInfoContainer::List TermGroupTable::getGroups()
{
    GroupInfoContainer::List ret;

    auto query = SqlQueryBuilder().selectAllGroups();
    DbTools::startQuery2(query);

    auto records = DbTools::getAllRecords(std::move(query));

    for (auto& record : records) {
        GroupInfoContainer info = sqlRecordToGroupInfo(record);
        ret.push_back(std::move(info));
    }

    return ret;
}

GroupInfoContainer TermGroupTable::sqlRecordToGroupInfo(const QSqlRecord& rec)
{
    GroupInfoContainer info;

    info.uuid    = QUuid(rec.value("uuid").toString());
    info.name    = rec.value("name").toString();
    info.comment = rec.value("comment").toString();

    return info;
}
