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

public:
    explicit Api(QObject* parent = nullptr);
    ~Api() = default;

private:
    FirstStartPreparer initPreparer;

    // Independent init
    QScopedPointer<NetworkManager> mNetwork;

    // Dependent init
    QScopedPointer<GroupsManager>         mGroupsManager;
    QScopedPointer<SyncManager>           mSyncManager;
    QScopedPointer<MainScene>             mScene;
};
