// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/Api.h"

#include <QQmlEngine>

#include "source/CommonTools/Platform.h"

#include "source/managers/notificationmanager.h"

Api::Api(QObject *parent)
    : QObject(parent)
    , mNetwork(new NetworkManager(NotificationManager::instance()))
    , mGroupsManager(new GroupsManager(NotificationManager::instance()))
    , mSyncManager(new SyncManager(mNetwork.get(), mGroupsManager.get()))
    , mScene(new MainScene(mGroupsManager.get()))
{}

NetworkManager *Api::network() const { return mNetwork.get(); }

SyncManager *Api::sync() const { return mSyncManager.get(); }

GroupsManager *Api::groups() const { return mGroupsManager.get(); }

MainScene *Api::scene() const { return mScene.get(); }

bool Api::readonlyMode() { return Platform::isWasm(); }
