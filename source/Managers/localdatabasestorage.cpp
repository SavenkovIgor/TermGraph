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

#include "source/Managers/localdatabasestorage.h"

#include <QMap>

LocalDatabaseStorage::LocalDatabaseStorage(const QString& filePath)
    : DataStorageInterface()
    , db(filePath)
{}

int LocalDatabaseStorage::storageVersion() const { return db.appConfigTable->getDbVersion(); }

UuidList LocalDatabaseStorage::getAllGroupsUuids(bool sortByLastEdit) const
{
    if (!sortByLastEdit) // Simple variant
        return db.groupTable->getAllUuids();

    // Load info from groups table - need if any group is empty and has no lastEdit value
    QMap<QUuid, QDateTime> groupsLastEdit;

    for (const auto& uuid : db.groupTable->getAllUuids())
        groupsLastEdit.insert(uuid, QDateTime());

    for (const auto& record : db.termTable->getAllLastEditRecords()) {
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

bool LocalDatabaseStorage::groupExist(const QUuid& groupUuid) const { return db.groupTable->groupExist(groupUuid); }

GroupInfoContainer LocalDatabaseStorage::getGroup(const QUuid& groupUuid) const
{
    return db.groupTable->getGroup(groupUuid);
}

GroupInfoContainer::List LocalDatabaseStorage::getGroups() const { return db.groupTable->getGroups(); }

bool LocalDatabaseStorage::addGroup(const GroupInfoContainer& groupInfo) { return db.groupTable->addGroup(groupInfo); }

bool LocalDatabaseStorage::updateGroup(const GroupInfoContainer& info) { return db.groupTable->updateGroup(info); }

void LocalDatabaseStorage::deleteGroup(const QUuid& groupUuid) { db.groupTable->deleteGroup(groupUuid); }

UuidList LocalDatabaseStorage::getAllNodesUuids(const QUuid& groupUuid) const
{
    return db.termTable->getAllNodesUuids(groupUuid);
}

bool LocalDatabaseStorage::nodeExist(const QUuid& nodeUuid) const { return db.termTable->nodeExist(nodeUuid); }

QUuid LocalDatabaseStorage::findNode(const QString& nodeName, const QUuid& groupUuid) const
{
    return db.termTable->nodeUuidForNameAndGroup(nodeName, groupUuid);
}

NodeInfoContainer LocalDatabaseStorage::getNode(const QUuid& nodeUuid) const
{
    return db.termTable->getNodeInfo(nodeUuid);
}

NodeInfoContainer::List LocalDatabaseStorage::getNodes(const QUuid& groupUuid) const
{
    return db.termTable->getAllNodesInfo(groupUuid);
}

NodeInfoContainer::List LocalDatabaseStorage::getNodes(const UuidList& nodesUuids) const { return {}; }

QDateTime LocalDatabaseStorage::getNodeLastEdit(const QUuid& nodeUuid) const
{
    return db.termTable->getLastEdit(nodeUuid);
}

bool LocalDatabaseStorage::addNode(const NodeInfoContainer& info) { return db.termTable->addNode(info); }

bool LocalDatabaseStorage::updateNode(const NodeInfoContainer&             info,
                                      DataStorageInterface::LastEditSource lastEditSource,
                                      bool                                 checkLastEdit)
{
    return db.termTable->updateNode(info, lastEditSource, checkLastEdit);
}

void LocalDatabaseStorage::deleteNode(const QUuid& nodeUuid) { return db.termTable->deleteTerm(nodeUuid); }
