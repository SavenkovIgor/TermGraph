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
    qmlRegisterModule("Api", 1, 0);
    qmlRegisterSingletonInstance("Api", 1, 0, "Api", this);

    qmlRegisterSingletonInstance("Api", 1, 0, "Scene", mScene.get());
    qmlRegisterSingletonInstance("Api", 1, 0, "GroupsManager", mGroupsManager.get());
}

NetworkManager *Api::network() const
{
    return mNetwork.get();
}

SyncManager *Api::sync() const
{
    return mSyncManager.get();
}
