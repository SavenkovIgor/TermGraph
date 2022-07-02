// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/managers/syncmanager.h"

SyncManager::SyncManager(NetworkManager* networkManager, GroupsManager* groupsManger, QObject* parent)
    : QObject(parent)
    , mNetworkManager(networkManager)
    , mGroupsManager(groupsManger)
{
    connect(this->mNetworkManager,
            &NetworkManager::newSyncGroup,
            this->mGroupsManager,
            &GroupsManager::importGroupFromJsonString);
}

bool SyncManager::isDataContainer(const QJsonDocument& doc)
{
    bool hasContentType = false;
    bool hasVersion     = false;
    bool hasContent     = false;

    auto docObject = doc.object();

    hasContentType = docObject.contains(contentTypeFieldName);
    hasVersion     = docObject.contains(versionFieldName);
    hasContent     = docObject.contains(contentFieldName);

    return hasContentType && hasVersion && hasContent;
}

void SyncManager::sendGroupByNetwork(const QUuid groupUuid)
{
    auto jsonDoc = mGroupsManager->getGroupForExport(groupUuid);
    mNetworkManager->sendGroup(jsonDoc);
}

QString SyncManager::getContentTypeName(const SyncManager::ContentType& type)
{
    switch (type) {
    case ContentType::groupsHashList: return "groupsHashList";
    case ContentType::undefined: return "";
    }

    return "";
}

SyncManager::ContentType SyncManager::getContentTypeByName(const QString& typeName)
{
    if (typeName == "groupsHashList") {
        return ContentType::groupsHashList;
    }

    return ContentType::undefined;
}
