// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/Api.h"

#include <QQmlEngine>

#include "source/CommonTools/Platform.h"

#include "source/managers/notificationmanager.h"

Api::Api(QObject *parent)
    : QObject(parent)
    , mNetwork(std::make_unique<NetworkManager>(NotificationManager::instance()))
    , mGroupsManager(std::make_unique<GroupsManager>(NotificationManager::instance()))
    , mSyncManager(std::make_unique<SyncManager>(mNetwork.get(), mGroupsManager.get()))
    , mScene(std::make_unique<MainScene>(mGroupsManager.get()))
{}

NetworkManager *Api::network() const { return mNetwork.get(); }

SyncManager *Api::sync() const { return mSyncManager.get(); }

GroupsManager *Api::groups() const { return mGroupsManager.get(); }

MainScene *Api::scene() const { return mScene.get(); }

bool Api::readonlyMode() { return Platform::isWasm(); }
