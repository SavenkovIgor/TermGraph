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

QUuid LocalDatabaseStorage::getFreeUuid() const
{
    while (true) {
        auto newUuid = QUuid::createUuid();
        if (!termExist(TermUuid::create(newUuid).value()) && !groupExist(GroupUuid::create(newUuid).value()))
            return newUuid;
    }
}

GroupUuid::List LocalDatabaseStorage::getAllGroupsUuids(bool sortByLastEdit) const
{
    if (!sortByLastEdit) // Simple variant
        return impl->db.groupTable->getAllUuids();

    // Load info from groups table - need if any group is empty and has no lastEdit value
    QMap<QUuid, QDateTime> groupsLastEdit;

    for (const auto& uuid : impl->db.groupTable->getAllUuids())
        groupsLastEdit.insert(uuid.get(), QDateTime());

    for (const auto& record : impl->db.termTable->getAllLastEditRecords()) {
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

    return ret;
}

bool LocalDatabaseStorage::groupExist(const GroupUuid& uuid) const { return impl->db.groupTable->groupExist(uuid); }

Result<GroupData> LocalDatabaseStorage::getGroup(const GroupUuid& uuid) const
{
    return impl->db.groupTable->getGroup(uuid);
}

GroupData::List LocalDatabaseStorage::getGroups() const { return impl->db.groupTable->getGroups(); }

Result<void> LocalDatabaseStorage::addGroup(const GroupData& info) { return impl->db.groupTable->addGroup(info); }

Result<void> LocalDatabaseStorage::updateGroup(const GroupData& info) { return impl->db.groupTable->updateGroup(info); }

Result<void> LocalDatabaseStorage::deleteGroup(const GroupUuid& uuid) { return impl->db.groupTable->deleteGroup(uuid); }

TermUuid::List LocalDatabaseStorage::getAllTermsUuids(Opt<GroupUuid> uuid) const
{
    return impl->db.termTable->getAllNodesUuids(uuid);
}

bool LocalDatabaseStorage::termExist(const TermUuid& uuid) const { return impl->db.termTable->nodeExist(uuid); }

Opt<TermUuid> LocalDatabaseStorage::findTerm(const QString& nodeName, const GroupUuid& uuid) const
{
    if (!impl->db.groupTable->groupExist(uuid))
        return std::nullopt;

    return impl->db.termTable->nodeUuidForNameAndGroup(nodeName, uuid);
}

Result<TermData> LocalDatabaseStorage::getTerm(const TermUuid& uuid) const
{
    return impl->db.termTable->getNodeInfo(uuid);
}

Result<TermData::List> LocalDatabaseStorage::getTerms(const GroupUuid& uuid) const
{
    if (!impl->db.groupTable->groupExist(uuid))
        return DbErrorCodes::UuidNotFound;

    return impl->db.termTable->getAllNodesInfo(uuid);
}

Result<TermData::List> LocalDatabaseStorage::getTerms(const UuidList& termsUuids) const { return TermData::List{}; }

Result<QDateTime> LocalDatabaseStorage::getTermLastEdit(const TermUuid& uuid) const
{
    return impl->db.termTable->getLastEdit(uuid);
}

Result<void> LocalDatabaseStorage::addTerm(const TermData& info)
{
    if (info.groupUuid.isNull())
        return DbErrorCodes::UuidEmpty;

    if (!impl->db.groupTable->groupExist(info.groupUuid))
        return DbErrorCodes::UuidNotFound;

    return impl->db.termTable->addNode(info);
}

Result<void> LocalDatabaseStorage::updateTerm(const TermData&                      info,
                                              DataStorageInterface::LastEditSource lastEditSource,
                                              bool                                 checkLastEdit)
{
    if (info.groupUuid.isNull())
        return DbErrorCodes::UuidEmpty;

    if (!impl->db.groupTable->groupExist(info.groupUuid))
        return DbErrorCodes::UuidNotFound;

    return impl->db.termTable->updateNode(info, lastEditSource, checkLastEdit);
}

Result<void> LocalDatabaseStorage::deleteTerm(const TermUuid& uuid) { return impl->db.termTable->deleteTerm(uuid); }
