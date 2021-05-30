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

bool NodesManager::addNewNode(const QJsonObject& object)
{
    assert(JsonTermDataParser::isValidKeysAndTypes(object));
    auto data = JsonTermDataParser::fromJson(object);

    if (!correctGroupUuid(data.groupUuid))
        return false;

    if (!correctNewNodeName(data.term, data.groupUuid))
        return false;

    dataStorage.addTerm(data);

    emit nodeChanged();

    return true;
}

bool NodesManager::changeNode(const QJsonObject& object)
{
    assert(JsonTermDataParser::isValidKeysAndTypes(object));
    auto data = JsonTermDataParser::fromJson(object);

    if (!correctGroupUuid(data.groupUuid))
        return false;

    assert(!data.uuid.isNull());

    if (data.uuid.isNull()) {
        NotificationManager::showWarning("Пустой uuid термина при попытке изменения");
        return false;
    }

    // Check for already existing node with same name
    auto alterNodeUuid = dataStorage.findTerm(data.term, data.groupUuid);
    if (!alterNodeUuid.isNull() && alterNodeUuid != data.uuid) {
        NotificationManager::showWarning("Термин с таким названием уже существует в этой группе");
        return false;
    }

    dataStorage.updateTerm(data, DataStorageInterface::LastEditSource::AutoGenerate, false);

    emit nodeChanged();
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
