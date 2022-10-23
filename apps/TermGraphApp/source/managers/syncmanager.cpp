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

    connect(this->mGroupsManager, &GroupsManager::exportGroupReady, this->mNetworkManager, &NetworkManager::sendGroup);
}

void SyncManager::sendGroupByNetwork(const QUuid groupUuid) { mGroupsManager->requestGroupExport(groupUuid); }
