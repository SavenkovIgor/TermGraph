// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QQmlEngine>

#include "source/helpers/firststartpreparer.h"
#include "source/MainScene.h"
#include "source/managers/groupsmanager.h"
#include "source/managers/syncmanager.h"

// Qml todo list
// TODO: Avoid implicit lookup in root scope (root.property)
// TODO: Use type annotations on all qml functions (function sum(a: int, b: int): int )
// TODO: Avoid duck typing
// TODO: Prefer id's over properties (prefer id instead parent)

// TODO: use QProperty https://doc.qt.io/qt-6/bindableproperties.html

class Api : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    // clang-format off
    Q_PROPERTY(NetworkManager* net    READ network CONSTANT FINAL)
    Q_PROPERTY(SyncManager*    sync   READ sync    CONSTANT FINAL)
    Q_PROPERTY(GroupsManager*  groups READ groups  CONSTANT FINAL)
    Q_PROPERTY(MainScene*      scene  READ scene   CONSTANT FINAL)
    // clang-format on

    Q_PROPERTY(bool readonlyMode READ readonlyMode CONSTANT FINAL)

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
    SyncManager*    sync() const;
    GroupsManager*  groups() const;
    MainScene*      scene() const;

    static bool readonlyMode();
};
