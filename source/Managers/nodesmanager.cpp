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

#include "source/Managers/nodesmanager.h"

#include "source/Managers/jsontermdataparser.h"

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

    TermData info;

    info.uuid        = QUuid();
    info.term        = name;
    info.definition  = def;
    info.description = descr;
    info.examples    = exam;
    info.groupUuid   = groupUuid;

    dataStorage.addTerm(info);

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
    auto alterNodeUuid = dataStorage.findTerm(name, groupUuid);
    if (!alterNodeUuid.isNull() && alterNodeUuid != nodeUuid) {
        NotificationManager::showWarning("Термин с таким названием уже существует в этой группе");
        return false;
    }

    TermData info;

    info.uuid        = nodeUuid;
    info.term        = name;
    info.definition  = definition;
    info.description = description;
    info.examples    = example;
    info.groupUuid   = groupUuid;

    dataStorage.updateTerm(info, DataStorageInterface::LastEditSource::AutoGenerate, false);

    if (sendChangeSignal) {
        emit nodeChanged();
    }

    return true;
}

void NodesManager::deleteNode(const QUuid uuid)
{
    dataStorage.deleteTerm(uuid);
    emit nodeChanged();
}

UuidList NodesManager::getAllNodesUuidsInGroup(const QUuid& groupUuid)
{
    return dataStorage.getAllTermsUuids(groupUuid);
}

void NodesManager::importNodeFromJson(const QJsonObject& nodeJson, bool importIfGroupNotExist)
{
    auto info = JsonTermDataParser::fromJson(nodeJson);

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
    if (!dataStorage.termExist(info.uuid)) {
        dataStorage.addTerm(info);
    } else {
        dataStorage.updateTerm(info, DataStorageInterface::LastEditSource::TakeFromTermData);
    }
}

bool NodesManager::correctGroupUuid(const QUuid& groupUuid)
{
    if (groupUuid.isNull()) {
        NotificationManager::showError("Пустой Uuid группы");
        return false;
    }

    if (!dataStorage.groupExist(groupUuid)) {
        NotificationManager::showError("Группа " + groupUuid.toString() + " не найдена");
        return false;
    }

    return true;
}

bool NodesManager::correctNewNodeName(const QString& name, QUuid& groupUuid)
{
    auto nodeUuid = dataStorage.findTerm(name, groupUuid);

    if (!nodeUuid.isNull()) {
        NotificationManager::showError("Термин с таким названием уже существует в этой группе");
        return false;
    }

    return true;
}
