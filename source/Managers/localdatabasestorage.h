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
#pragma once

#include "source/Database/database.h"
#include "source/Helpers/appconfig.h"
#include "source/Managers/datastorageinterface.h"
#include "source/Model/Termin/nodeinfocontainer.h"
#include "source/Model/TerminGroup/groupinfocontainer.h"

class LocalDatabaseStorage : public DataStorageInterface
{
public:
    LocalDatabaseStorage(const QString& filePath = AppSettings::StdPaths::defaultDatabaseFilePath());

    // DataStorageInterface interface
public:
    int storageVersion() const override;

    UuidList getAllGroupsUuids(bool sortByLastEdit) const override;

    // Add getFreeUuid for groups
    bool                     groupExist(const QUuid& groupUuid) const override;
    GroupInfoContainer       getGroup(const QUuid& groupUuid) const override;
    GroupInfoContainer::List getGroups() const override;

    bool addGroup(const GroupInfoContainer& groupInfo) override;
    bool updateGroup(const GroupInfoContainer& info) override;
    void deleteGroup(const QUuid& groupUuid) override;

    UuidList getAllNodesUuids(const QUuid& groupUuid) const override;

    // Add getFreeUuid for nodes
    bool                    nodeExist(const QUuid& nodeUuid) const override;
    QUuid                   findNode(const QString& nodeName, const QUuid& groupUuid) const override;
    NodeInfoContainer       getNode(const QUuid& nodeUuid) const override;
    NodeInfoContainer::List getNodes(const QUuid& groupUuid) const override;
    NodeInfoContainer::List getNodes(const UuidList& nodesUuids) const override;
    QDateTime               getNodeLastEdit(const QUuid& nodeUuid) const override;

    bool addNode(const NodeInfoContainer& info) override;
    bool updateNode(const NodeInfoContainer& info, LastEditSource lastEditSource, bool checkLastEdit) override;
    void deleteNode(const QUuid& nodeUuid) override;

private:
    Database db;
};
