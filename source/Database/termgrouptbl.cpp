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

#include "source/Database/termgrouptbl.h"

#include "source/Database/columns/termgroupcolumn.h"
#include "source/Database/dbtablenames.h"
#include "source/Database/sqlqueryconstructor.h"
#include "source/Database/tools/dbtools.h"

bool TermGroupTable::addGroup(const GroupInfoContainer& info)
{
    QUuid groupUuid = info.uuid;

    if (groupUuid.isNull())
        groupUuid = generateNewUuid();

    if (info.name.simplified().isEmpty())
        return false;

    if (groupWithNameExist(info.name))
        return false;

    InsertContainer::List values;

    values.push_back(InsertContainer(TermGroupColumn::uuid, groupUuid.toString()));
    values.push_back(InsertContainer(TermGroupColumn::name, info.name));
    values.push_back(InsertContainer(TermGroupColumn::comment, info.comment));

    return insertInto(values);
}

bool TermGroupTable::updateGroup(const GroupInfoContainer& info)
{
    if (info.uuid.isNull())
        return false;

    if (!groupExist(info.uuid))
        return false;

    SetExpression set;

    set.set(TermGroupColumn::name, info.name);
    set.set(TermGroupColumn::comment, info.comment);

    updateWhere(set, whereUuidEqual(info.uuid));

    return true;
}

UuidList TermGroupTable::getAllUuids()
{
    UuidList ret;
    auto     records = toRecVector(select(TermGroupColumn::uuid));

    for (const auto& record : records) {
        QUuid uuid(record.value(TermGroupColumn::uuid).toString());
        if (!uuid.isNull()) {
            ret.push_back(uuid);
        }
    }

    return ret;
}

void TermGroupTable::deleteGroup(const QUuid& groupUuid)
{
    auto query = SqlQueryConstructor::deleteGroup(groupUuid);
    DbTools::startQuery(query);
}

bool TermGroupTable::groupExist(const QUuid& uuid) { return hasAnyRecord(whereUuidEqual(uuid)); }

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
    auto      where = WhereCondition::columnEqual(TermGroupColumn::name, groupName);
    QSqlQuery q     = select(TermGroupColumn::uuid, where);
    if (!q.next()) {
        return QUuid();
    }

    return QUuid(q.record().value(TermGroupColumn::uuid).toString());
}

bool TermGroupTable::groupWithNameExist(const QString& groupName) { return !getUuid(groupName).isNull(); }

const char* TermGroupTable::tableName() const { return TableName::GROUPS; }

TColumn TermGroupTable::primaryKey() const { return TermGroupColumn::uuid; }

void TermGroupTable::initTable() { createTable(); }

TColumn::List TermGroupTable::getAllColumns() const
{
    TColumn::List lst;

    for (const auto& column : TermGroupColumn::columns)
        lst << column;

    return lst;
}

GroupInfoContainer TermGroupTable::getGroup(const QUuid& uuid)
{
    QSqlQuery sel = select(getAllColumns(), whereUuidEqual(uuid));

    if (!sel.next())
        return GroupInfoContainer();

    auto rec = sel.record();

    return sqlRecordToGroupInfo(rec);
}

GroupInfoContainer::List TermGroupTable::getGroups()
{
    GroupInfoContainer::List ret;

    auto sel     = select(getAllColumns());
    auto records = toRecVector(std::move(sel));

    for (auto& record : records) {
        GroupInfoContainer info = sqlRecordToGroupInfo(record);
        ret.push_back(std::move(info));
    }

    return ret;
}

WhereCondition TermGroupTable::whereUuidEqual(const QUuid& uuid) { return primaryKeyEqual(uuid.toString()); }

GroupInfoContainer TermGroupTable::sqlRecordToGroupInfo(const QSqlRecord& rec)
{
    GroupInfoContainer info;

    info.uuid    = QUuid(rec.value(TermGroupColumn::uuid).toString());
    info.name    = QueryTools::unVV(rec.value(TermGroupColumn::name).toString());
    info.comment = QueryTools::unVV(rec.value(TermGroupColumn::comment).toString());

    return info;
}
