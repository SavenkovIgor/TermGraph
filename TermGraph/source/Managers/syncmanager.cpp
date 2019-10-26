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

#include "syncmanager.h"

SyncManager::SyncManager(NetworkManager *networkManager,
                         GroupsManager *groupsManger,
                         NodesManager *nodesManager,
                         QObject *parent)
    : QObject(parent)
{
    this->networkManager = networkManager;
    this->groupsManager = groupsManger;
    this->nodesManager = nodesManager;

    connect(this->networkManager,
            &NetworkManager::newSyncGroup,
            this->groupsManager,
            &GroupsManager::importGroupFromJsonString);
}

bool SyncManager::isDataContainer(const QJsonDocument &doc)
{
    bool hasContentType = false;
    bool hasVersion = false;
    bool hasContent = false;

    auto docObject = doc.object();

    hasContentType = docObject.contains(contentTypeFieldName);
    hasVersion = docObject.contains(versionFieldName);
    hasContent = docObject.contains(contentFieldName);

    return hasContentType && hasVersion && hasContent;
}

void SyncManager::sendGroupByNetwork(const QString &groupUuid)
{
    auto jsonDoc = groupsManager->getGroupForExport(groupUuid);
    networkManager->sendGroup(jsonDoc);
}

QString SyncManager::getContentTypeName(const SyncManager::ContentType &type)
{
    switch (type) {
    case ContentType::groupsHashList:
        return "groupsHashList";
    case ContentType::undefined:
        return "";
    }

    return "";
}

SyncManager::ContentType SyncManager::getContentTypeByName(const QString &typeName)
{
    if (typeName == "groupsHashList") {
        return ContentType::groupsHashList;
    }

    return ContentType::undefined;
}
