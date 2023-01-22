// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>

#include "source/managers/groupsmanager.h"
#include "source/managers/networkmanager.h"

class SyncManager : public QObject
{
    Q_OBJECT

public:
    explicit SyncManager(NetworkManager* networkManager, GroupsManager* groupsManger, QObject* parent = nullptr);

    Q_INVOKABLE void sendGroupByNetwork(const QUuid groupUuid);

private:
    NetworkManager* mNetworkManager;
    GroupsManager*  mGroupsManager;
};
