/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
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

LocalDatabaseStorage::LocalDatabaseStorage(Database& db)
    : DataStorageInterface()
    , db(db)
{}

int LocalDatabaseStorage::storageVersion() const
{
    return db.appConfigTable->getDbVersion();
}

UuidList LocalDatabaseStorage::getAllGroupsUuids() const
{
    return db.groupTable->getAllUuids();
}

UuidList LocalDatabaseStorage::getAllGroupsUuidsSortedByLastEdit() const
{
    return {};
}

bool LocalDatabaseStorage::groupExist(const QUuid& groupUuid) const
{
    return db.groupTable->groupExist(groupUuid);
}

GroupInfoContainer LocalDatabaseStorage::getGroup(const QUuid& groupUuid) const
{
    return db.groupTable->getGroup(groupUuid);
}

GroupInfoContainer::List LocalDatabaseStorage::getGroups() const
{
    return db.groupTable->getGroups();
}

bool LocalDatabaseStorage::addGroup(const GroupInfoContainer& groupInfo)
{
    return db.groupTable->addGroup(groupInfo);
}

bool LocalDatabaseStorage::updateGroup(const GroupInfoContainer& info)
{
    return db.groupTable->updateGroup(info);
}

void LocalDatabaseStorage::deleteGroup(const QUuid& groupUuid)
{
    db.groupTable->deleteGroup(groupUuid);
}

UuidList LocalDatabaseStorage::getAllNodesUuids(const QUuid& groupUuid) const
{
    return db.nodeTable->getAllNodesUuids(groupUuid);
}

bool LocalDatabaseStorage::nodeExist(const QUuid& nodeUuid) const
{
    return db.nodeTable->nodeExist(nodeUuid);
}

QUuid LocalDatabaseStorage::findNode(const QString& nodeName, QUuid& groupUuid) const
{
    return db.nodeTable->nodeUuidForNameAndGroup(nodeName, groupUuid);
}

NodeInfoContainer LocalDatabaseStorage::getNode(const QUuid& nodeUuid) const
{
    return db.nodeTable->getNode(nodeUuid);
}

NodeInfoContainer::List LocalDatabaseStorage::getNodes(const QUuid& groupUuid) const
{
    return db.nodeTable->getAllNodesInfo(groupUuid);
}

NodeInfoContainer::List LocalDatabaseStorage::getNodes(const UuidList* nodesUuids) const
{
    return {};
}

QDateTime LocalDatabaseStorage::getNodeLastEdit(const QUuid& nodeUuid) const
{
    return db.nodeTable->getLastEdit(nodeUuid);
}

QUuid LocalDatabaseStorage::addNode(const NodeInfoContainer& info)
{
    return db.nodeTable->addNode(info);
}

bool LocalDatabaseStorage::updateNode(const NodeInfoContainer&             info,
                                      DataStorageInterface::LastEditSource lastEditSource,
                                      bool                                 checkLastEdit)
{
    return db.nodeTable->updateNode(info, lastEditSource, checkLastEdit);
}

void LocalDatabaseStorage::deleteNode(const QUuid& nodeUuid)
{
    return db.nodeTable->deleteNode(nodeUuid);
}
