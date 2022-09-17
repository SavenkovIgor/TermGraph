// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/api.h"

#include <QQmlEngine>

#include "source/managers/notificationmanager.h"


Api::Api(QObject* parent)
    : QObject(parent)
    , initPreparer()
    , mNetwork(new NetworkManager(NotificationManager::instance()))
    , mGroupsManager(new GroupsManager(NotificationManager::instance()))
    , mSyncManager(new SyncManager(mNetwork.get(), mGroupsManager.get()))
    , mScene(new MainScene(mGroupsManager.get()))
{
}

NetworkManager *Api::network() const
{
    return mNetwork.get();
}

SyncManager *Api::sync() const
{
    return mSyncManager.get();
}

GroupsManager *Api::groups() const
{
    return mGroupsManager.get();
}

MainScene *Api::scene() const
{
    return mScene.get();
}
