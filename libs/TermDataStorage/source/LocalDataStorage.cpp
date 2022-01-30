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

#include "include/TermDataStorage/LocalDataStorage.h"

#include <QMap>

#include "source/Database.h"

class StorageImpl
{
public:
    StorageImpl(const QString& filePath, const QString& backupFolderPath)
        : db(filePath, backupFolderPath)
    {}

    Database db;
};

LocalDatabaseStorage::LocalDatabaseStorage(const QString& filePath, const QString& backupFolderPath)
    : DataStorageInterface()
    , impl(new StorageImpl(filePath, backupFolderPath))
{}

int LocalDatabaseStorage::storageVersion() const { return impl->db.appConfigTable->getDbVersion(); }

FutureRes<GroupUuid::List> LocalDatabaseStorage::getAllGroupsUuids(bool sortByLastEdit) const
{
    // Simple variant
    if (!sortByLastEdit) {
        return toPromise<Result<GroupUuid::List>>([this] { return impl->db.groupTable->allUuids(); });
    }

    // Load info from groups table - need if any group is empty and has no lastEdit value
    QMap<QUuid, QDateTime> groupsLastEdit;

    for (const auto& uuid : impl->db.groupTable->allUuids())
        groupsLastEdit.insert(uuid.get(), QDateTime());

    for (const auto& record : impl->db.termTable->allLastEditRecords()) {
        QUuid     groupUuid = QUuid(record.value("groupUuid").toString());
        QDateTime lastEdit  = QDateTime::fromString(record.value("lastEdit").toString(), Qt::ISODate);

        if (groupsLastEdit.contains(groupUuid)) {
            if (groupsLastEdit[groupUuid].isNull()) {
                groupsLastEdit[groupUuid] = lastEdit;
            } else {
                groupsLastEdit[groupUuid] = std::max(groupsLastEdit[groupUuid], lastEdit);
            }
        }
    }

    // Casting to pairs
    QList<QPair<QUuid, QDateTime>> groupSorting;

    // Forming structure with group uuids and last edit times
    for (auto& [groupUuid, lastEdit] : groupsLastEdit.toStdMap()) {
        groupSorting.append(QPair<QUuid, QDateTime>(groupUuid, lastEdit));
    }

    // Sorting this structure
    auto groupOrdering = [](const auto& g1, const auto& g2) { return g1.second > g2.second; };

    std::sort(groupSorting.begin(), groupSorting.end(), groupOrdering);

    // Casting back to uuids only
    GroupUuid::List ret;

    for (const auto& group : groupSorting)
        if (auto gUuid = GroupUuid::create(group.first))
            ret.push_back(*gUuid);

    return toPromise<Result<GroupUuid::List>>([&ret] { return ret; });
}

FutureRes<GroupData> LocalDatabaseStorage::getGroup(const GroupUuid& uuid) const
{
    return toPromise<Result<GroupData>>([this, &uuid] { return impl->db.groupTable->group(uuid); });
}

FutureRes<GroupData::List> LocalDatabaseStorage::getGroups() const
{
    return toPromise<Result<GroupData::List>>([this] { return impl->db.groupTable->allGroups(); });
}

FutureRes<GroupData> LocalDatabaseStorage::addGroup(const GroupData& info)
{
    return toPromise<Result<GroupData>>([this, info] { return impl->db.groupTable->addGroup(info); });
}

FutureRes<GroupData> LocalDatabaseStorage::updateGroup(const GroupData& info)
{
    return toPromise<Result<GroupData>>([this, info] { return impl->db.groupTable->updateGroup(info); });
}

FutureRes<GroupData> LocalDatabaseStorage::deleteGroup(const GroupUuid& uuid)
{
    return toPromise<Result<GroupData>>([this, uuid] { return impl->db.groupTable->deleteGroup(uuid); });
}

FutureRes<TermUuid::List> LocalDatabaseStorage::getAllTermsUuids(Opt<GroupUuid> uuid) const
{
    return toPromise<Result<TermUuid::List>>([this, uuid] { return impl->db.termTable->allUuids(uuid); });
}

FutureRes<TermData> LocalDatabaseStorage::getTerm(const QString& nodeName, const GroupUuid& uuid) const
{
    if (!impl->db.groupTable->exist(uuid))
        return toPromise<Result<TermData>>([] { return DbErrorCodes::GroupUuidNotFound; });

    return toPromise<Result<TermData>>([this, nodeName, uuid] { return impl->db.termTable->term(nodeName, uuid); });
}

FutureRes<TermData> LocalDatabaseStorage::getTerm(const TermUuid& uuid) const
{
    return toPromise<Result<TermData>>([this, uuid] { return impl->db.termTable->term(uuid); });
}

FutureRes<TermData::List> LocalDatabaseStorage::getTerms(const GroupUuid& uuid) const
{
    if (!impl->db.groupTable->exist(uuid))
        return toPromise<Result<TermData::List>>([] { return DbErrorCodes::GroupUuidNotFound; });

    return toPromise<Result<TermData::List>>([this, uuid] { return impl->db.termTable->allTerms(uuid); });
}

FutureRes<QDateTime> LocalDatabaseStorage::getTermLastEdit(const TermUuid& uuid) const
{
    return toPromise<Result<QDateTime>>([this, uuid] { return impl->db.termTable->lastEdit(uuid); });
}

Result<void> LocalDatabaseStorage::addTerm(const TermData& info)
{
    if (!impl->db.groupTable->exist(info.groupUuid))
        return DbErrorCodes::GroupUuidNotFound;

    return impl->db.termTable->addTerm(info);
}

Result<void> LocalDatabaseStorage::updateTerm(const TermData&                      info,
                                              DataStorageInterface::LastEditSource lastEditSource,
                                              bool                                 checkLastEdit)
{
    if (!impl->db.groupTable->exist(info.groupUuid))
        return DbErrorCodes::GroupUuidNotFound;

    return impl->db.termTable->updateTerm(info, lastEditSource, checkLastEdit);
}

Result<void> LocalDatabaseStorage::deleteTerm(const TermUuid& uuid) { return impl->db.termTable->deleteTerm(uuid); }
