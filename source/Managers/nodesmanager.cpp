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

#include "source/Managers/nodesmanager.h"

#include "source/Managers/jsonnodeinfocontainerparser.h"
#include "source/databaseWorks/columns/nodecolumn.h"

NodesManager::NodesManager(QObject* parent)
    : QObject(parent)
{}

bool NodesManager::addNewNode(const QString& name,
                              const QString& def,
                              const QString& descr,
                              const QString& exam,
                              const QString& groupUuidString,
                              const bool&    sendChangeSignal)
{
    auto groupUuid = QUuid(groupUuidString);

    if (!correctGroupUuid(groupUuid))
        return false;

    if (!correctNewNodeName(name, groupUuid))
        return false;

    QUuid nodeUuid = Database::instance().nodeTable->addNode(name, groupUuid);
    return changeNode(nodeUuid, name, def, descr, exam, groupUuid, sendChangeSignal);
}

bool NodesManager::changeNode(const QUuid&   nodeUuid,
                              const QString& name,
                              const QString& definition,
                              const QString& description,
                              const QString& example,
                              const QString& groupUuidString,
                              const bool&    sendChangeSignal)
{
    auto groupUuid = QUuid(groupUuidString);

    if (!correctGroupUuid(groupUuid))
        return false;

    // Check for already existing node with same name
    auto alterNodeUuid = getNodeUuidByNameAndGroup(name, groupUuid);
    if (!alterNodeUuid.isNull()) {
        if (alterNodeUuid != nodeUuid) {
            NotificationManager::showWarning("Термин с таким названием уже существует в этой группе");
            return false;
        }
    }

    return changeNode(nodeUuid, name, definition, description, example, groupUuid, sendChangeSignal);
}

bool NodesManager::changeNode(const QUuid&   nodeUuid,
                              const QString& name,
                              const QString& definition,
                              const QString& description,
                              const QString& example,
                              const QUuid&   groupUuid,
                              const bool&    sendChangeSignal)
{
    auto& db = Database::instance();

    NodeInfoContainer info;

    info.uuid        = nodeUuid;
    info.term        = name;
    info.definition  = definition;
    info.description = description;
    info.examples    = example;
    info.groupUuid   = groupUuid;

    db.nodeTable->updateNode(info, NodeTable::LastEditSource::AutoGenerate, false);

    if (sendChangeSignal) {
        emit nodeChanged();
    }

    return true;
}

void NodesManager::deleteNode(QUuid uuid)
{
    Database::instance().nodeTable->deleteNode(uuid);
    emit nodeChanged();
}

PaintedTerm::List NodesManager::getAllNodesForGroup(const QUuid& groupUuid)
{
    PaintedTerm::List ret;
    auto              nodesInfo = Database::instance().nodeTable->getAllNodesInfo(groupUuid);

    for (const auto& info : nodesInfo)
        ret << new PaintedTerm(info);

    return ret;
}

UuidList NodesManager::getAllNodesUuidsInGroup(const QUuid& groupUuid)
{
    return Database::instance().nodeTable->getAllNodesUuids(groupUuid);
}

QDateTime NodesManager::getLastEdit(QUuid nodeUuid)
{
    return Database::instance().nodeTable->getLastEdit(nodeUuid);
}

void NodesManager::importNodeFromJson(const QJsonObject& nodeJson, bool importIfGroupNotExist)
{
    auto& db   = Database::instance();
    auto  info = JsonNodeInfoContainerParser::fromJson(nodeJson);

    if (info.uuid.isNull())
        return;

    if (info.groupUuid.isNull())
        return;

    if (!db.groupTable->hasGroupWithUuid(info.groupUuid)) {
        if (!importIfGroupNotExist) {
            return;
        }
    }

    // Create
    if (!db.nodeTable->hasNodeWithUuid(info.uuid)) {
        db.nodeTable->addNode(info);
    } else {
        db.nodeTable->updateNode(info, NodeTable::LastEditSource::TakeFromNodeInfo);
    }
}

QJsonObject NodesManager::getNodeJson(const QUuid& uuid)
{
    auto info = Database::instance().nodeTable->getNode(uuid);
    return JsonNodeInfoContainerParser::toJson(info);
}

bool NodesManager::correctGroupUuid(const QUuid& groupUuid, bool sendWarnings)
{
    if (groupUuid.isNull()) {
        if (sendWarnings) {
            NotificationManager::showWarning("Пустой или некорректный Uuid группы");
        }
        return false;
    }

    if (!Database::instance().groupTable->hasGroupWithUuid(groupUuid)) {
        if (sendWarnings) {
            NotificationManager::showWarning("Группа " + groupUuid.toString() + " не найдена");
        }
        return false;
    }

    return true;
}

bool NodesManager::correctNewNodeName(const QString& name, QUuid& groupUuid, bool showWarnings)
{
    if (hasNodeWithNameInGroup(name, groupUuid)) {
        if (showWarnings) {
            NotificationManager::showWarning("Термин с таким названием уже существует в этой группе");
        }
        return false;
    }

    return true;
}

QUuid NodesManager::getNodeUuidByNameAndGroup(const QString& name, QUuid& groupUuid) const
{
    return Database::instance().nodeTable->nodeUuidForNameAndGroup(name, groupUuid);
}

bool NodesManager::hasNodeWithNameInGroup(const QString& name, QUuid& groupUuid) const
{
    return !getNodeUuidByNameAndGroup(name, groupUuid).isNull();
}