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

NodesManager::NodesManager(DataStorageInterface& dataStorage, QObject* parent)
    : QObject(parent)
    , dataStorage(dataStorage)
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

    NodeInfoContainer info;

    info.uuid        = QUuid();
    info.term        = name;
    info.definition  = def;
    info.description = descr;
    info.examples    = exam;
    info.groupUuid   = groupUuid;

    dataStorage.addNode(info);

    if (sendChangeSignal)
        emit nodeChanged();

    return true;
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

    assert(!nodeUuid.isNull());

    if (nodeUuid.isNull()) {
        NotificationManager::showWarning("Пустой uuid термина при попытке изменения");
        return false;
    }

    // Check for already existing node with same name
    auto alterNodeUuid = dataStorage.findNode(name, groupUuid);
    if (alterNodeUuid != nodeUuid) {
        NotificationManager::showWarning("Термин с таким названием уже существует в этой группе");
        return false;
    }

    NodeInfoContainer info;

    info.uuid        = nodeUuid;
    info.term        = name;
    info.definition  = definition;
    info.description = description;
    info.examples    = example;
    info.groupUuid   = groupUuid;

    dataStorage.updateNode(info, DataStorageInterface::LastEditSource::AutoGenerate, false);

    if (sendChangeSignal) {
        emit nodeChanged();
    }

    return true;
}

void NodesManager::deleteNode(QUuid uuid)
{
    dataStorage.deleteNode(uuid);
    emit nodeChanged();
}

PaintedTerm::List NodesManager::getAllNodesForGroup(const QUuid& groupUuid)
{
    PaintedTerm::List ret;

    auto nodesInfo = dataStorage.getNodes(groupUuid);

    for (const auto& info : nodesInfo)
        ret << new PaintedTerm(info);

    return ret;
}

UuidList NodesManager::getAllNodesUuidsInGroup(const QUuid& groupUuid)
{
    return dataStorage.getAllNodesUuids(groupUuid);
}

QDateTime NodesManager::getLastEdit(QUuid nodeUuid)
{
    return dataStorage.getNodeLastEdit(nodeUuid);
}

void NodesManager::importNodeFromJson(const QJsonObject& nodeJson, bool importIfGroupNotExist)
{
    auto  info = JsonNodeInfoContainerParser::fromJson(nodeJson);

    if (info.uuid.isNull())
        return;

    if (info.groupUuid.isNull())
        return;

    if (!dataStorage.groupExist(info.groupUuid)) {
        if (!importIfGroupNotExist) {
            return;
        }
    }

    // Create
    if (!dataStorage.nodeExist(info.uuid)) {
        dataStorage.addNode(info);
    } else {
        dataStorage.updateNode(info, DataStorageInterface::LastEditSource::TakeFromNodeInfo);
    }
}

QJsonObject NodesManager::getNodeJson(const QUuid& uuid)
{
    auto info = dataStorage.getNode(uuid);
    return JsonNodeInfoContainerParser::toJson(info);
}

bool NodesManager::correctGroupUuid(const QUuid& groupUuid)
{
    if (groupUuid.isNull()) {
        NotificationManager::showWarning("Пустой Uuid группы");
        return false;
    }

    if (!dataStorage.groupExist(groupUuid)) {
        NotificationManager::showWarning("Группа " + groupUuid.toString() + " не найдена");
        return false;
    }

    return true;
}

bool NodesManager::correctNewNodeName(const QString& name, QUuid& groupUuid)
{
    auto nodeUuid = dataStorage.findNode(name, groupUuid);

    if (!nodeUuid.isNull()) {
        NotificationManager::showWarning("Термин с таким названием уже существует в этой группе");
        return false;
    }

    return true;
}
