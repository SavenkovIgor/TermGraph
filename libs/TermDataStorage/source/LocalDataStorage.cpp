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
        if (!termExist(newUuid) && !groupExist(newUuid))
            return newUuid;
    }
}

UuidList LocalDatabaseStorage::getAllGroupsUuids(bool sortByLastEdit) const
{
    if (!sortByLastEdit) // Simple variant
        return impl->db.groupTable->getAllUuids();

    // Load info from groups table - need if any group is empty and has no lastEdit value
    QMap<QUuid, QDateTime> groupsLastEdit;

    for (const auto& uuid : impl->db.groupTable->getAllUuids())
        groupsLastEdit.insert(uuid, QDateTime());

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
    UuidList ret;

    for (const auto& group : groupSorting)
        ret.push_back(group.first);

    return ret;
}

bool LocalDatabaseStorage::groupExist(const QUuid& groupUuid) const
{
    return impl->db.groupTable->groupExist(groupUuid);
}

result<GroupData> LocalDatabaseStorage::getGroup(const QUuid& groupUuid) const
{
    return impl->db.groupTable->getGroup(groupUuid);
}

GroupData::List LocalDatabaseStorage::getGroups() const { return impl->db.groupTable->getGroups(); }

result<void> LocalDatabaseStorage::addGroup(const GroupData& info) { return impl->db.groupTable->addGroup(info); }

result<void> LocalDatabaseStorage::updateGroup(const GroupData& info) { return impl->db.groupTable->updateGroup(info); }

result<void> LocalDatabaseStorage::deleteGroup(const QUuid& groupUuid)
{
    return impl->db.groupTable->deleteGroup(groupUuid);
}

UuidList LocalDatabaseStorage::getAllTermsUuids(const QUuid& groupUuid) const
{
    return impl->db.termTable->getAllNodesUuids(groupUuid);
}

bool LocalDatabaseStorage::termExist(const QUuid& nodeUuid) const { return impl->db.termTable->nodeExist(nodeUuid); }

result<QUuid> LocalDatabaseStorage::findTerm(const QString& nodeName, const QUuid& groupUuid) const
{
    if (groupUuid.isNull())
        return DbErrorCodes::UuidEmpty;

    if (!impl->db.groupTable->groupExist(groupUuid))
        return DbErrorCodes::UuidNotExist;

    return impl->db.termTable->nodeUuidForNameAndGroup(nodeName, groupUuid);
}

result<TermData> LocalDatabaseStorage::getTerm(const QUuid& nodeUuid) const
{
    return impl->db.termTable->getNodeInfo(nodeUuid);
}

result<TermData::List> LocalDatabaseStorage::getTerms(const QUuid& groupUuid) const
{
    if (groupUuid.isNull())
        return DbErrorCodes::UuidEmpty;

    if (!impl->db.groupTable->groupExist(groupUuid))
        return DbErrorCodes::UuidNotExist;

    return impl->db.termTable->getAllNodesInfo(groupUuid);
}

result<TermData::List> LocalDatabaseStorage::getTerms(const UuidList& termsUuids) const
{
    return DbErrorCodes::UuidNotExist;
}

result<QDateTime> LocalDatabaseStorage::getTermLastEdit(const QUuid& nodeUuid) const
{
    return impl->db.termTable->getLastEdit(nodeUuid);
}

result<void> LocalDatabaseStorage::addTerm(const TermData& info)
{
    if (info.groupUuid.isNull())
        return DbErrorCodes::UuidEmpty;

    if (!impl->db.groupTable->groupExist(info.groupUuid))
        return DbErrorCodes::UuidNotExist;

    return impl->db.termTable->addNode(info);
}

result<void> LocalDatabaseStorage::updateTerm(const TermData&                      info,
                                              DataStorageInterface::LastEditSource lastEditSource,
                                              bool                                 checkLastEdit)
{
    if (info.groupUuid.isNull())
        return DbErrorCodes::UuidEmpty;

    if (!impl->db.groupTable->groupExist(info.groupUuid))
        return DbErrorCodes::UuidNotExist;

    return impl->db.termTable->updateNode(info, lastEditSource, checkLastEdit);
}

result<void> LocalDatabaseStorage::deleteTerm(const QUuid& nodeUuid)
{
    return impl->db.termTable->deleteTerm(nodeUuid);
}
