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

#include <QDateTime>
#include <QString>
#include <QUuid>

#include "source/Helpers/handytypes.h"
#include "source/Model/Termin/nodeinfocontainer.h"
#include "source/Model/TerminGroup/groupinfocontainer.h"

// Interface
class DataStorageInterface
{
public:
    enum LastEditSource { TakeFromNodeInfo = 0, AutoGenerate };

    // Storage
    virtual int storageVersion() const = 0;

    // Groups
    virtual UuidList getAllGroupsUuids(bool sortByLastEdit = false) const = 0;

    virtual bool                     groupExist(const QUuid& groupUuid) const = 0;
    virtual GroupInfoContainer       getGroup(const QUuid& groupUuid) const   = 0;
    virtual GroupInfoContainer::List getGroups() const                        = 0;

    virtual bool addGroup(const GroupInfoContainer& groupInfo) = 0;
    virtual bool updateGroup(const GroupInfoContainer& info)   = 0;
    virtual void deleteGroup(const QUuid& groupUuid)           = 0;

    // Nodes
    virtual UuidList getAllNodesUuids(const QUuid& groupUuid = QUuid()) const = 0;

    virtual bool                    nodeExist(const QUuid& nodeUuid) const                          = 0;
    virtual QUuid                   findNode(const QString& nodeName, const QUuid& groupUuid) const = 0;
    virtual NodeInfoContainer       getNode(const QUuid& nodeUuid) const                            = 0;
    virtual NodeInfoContainer::List getNodes(const QUuid& groupUuid) const                          = 0;
    virtual NodeInfoContainer::List getNodes(const UuidList* nodesUuids) const                      = 0;
    virtual QDateTime               getNodeLastEdit(const QUuid& nodeUuid) const                    = 0;

    virtual bool addNode(const NodeInfoContainer& info) = 0;
    virtual bool updateNode(const NodeInfoContainer& info, LastEditSource lastEditSource, bool checkLastEdit = true) = 0;
    virtual void deleteNode(const QUuid& nodeUuid) = 0;
};
