// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "source/helpers/firststartpreparer.h"
#include "source/mainscene.h"
#include "source/managers/groupsmanager.h"
#include "source/managers/syncmanager.h"

class Application : public QObject
{
    Q_OBJECT

public:
    explicit Application(QObject* parent = nullptr);
    ~Application() = default;

private:
    FirstStartPreparer initPreparer;

    // Independent init
    QScopedPointer<NetworkManager> network;

    // Dependent init
    QScopedPointer<GroupsManager>         groupsManager;
    QScopedPointer<SyncManager>           syncManager;
    QScopedPointer<MainScene>             scene;
    QScopedPointer<QQmlApplicationEngine> qmlEngine;
};
