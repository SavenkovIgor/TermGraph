// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>

#include "source/helpers/firststartpreparer.h"
#include "source/mainscene.h"
#include "source/managers/groupsmanager.h"
#include "source/managers/syncmanager.h"


class Api : public QObject
{
    Q_OBJECT

    Q_PROPERTY(NetworkManager* network READ network CONSTANT FINAL)
    Q_PROPERTY(SyncManager*    sync    READ sync    CONSTANT FINAL)
    Q_PROPERTY(GroupsManager*  groups  READ groups  CONSTANT FINAL)

public:
    explicit Api(QObject* parent = nullptr);
    ~Api() = default;

private:
    FirstStartPreparer initPreparer;

    // Independent init
    QScopedPointer<NetworkManager> mNetwork;

    // Dependent init
    QScopedPointer<GroupsManager> mGroupsManager;
    QScopedPointer<SyncManager>   mSyncManager;
    QScopedPointer<MainScene>     mScene;

    NetworkManager* network() const;
    SyncManager* sync() const;
    GroupsManager* groups() const;
};
