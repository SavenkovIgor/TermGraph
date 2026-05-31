// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/Api.hpp"

#include <QQmlEngine>

#include "source/CommonTools/Platform.hpp"

#include "source/managers/notificationmanager.hpp"

Api::Api(QObject *parent)
    : QObject(parent)
    , mNetwork(std::make_unique<NetworkManager>(NotificationManager::instance()))
    , mGroupsManager(std::make_unique<GroupsManager>(NotificationManager::instance()))
    , mScene(std::make_unique<MainScene>(mGroupsManager.get()))
{}

NetworkManager *Api::network() const { return mNetwork.get(); }

GroupsManager *Api::groups() const { return mGroupsManager.get(); }

MainScene *Api::scene() const { return mScene.get(); }

bool Api::readonlyMode() { return Platform::isWasm(); }
