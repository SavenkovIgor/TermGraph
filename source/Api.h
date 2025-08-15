// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <memory>

#include <QObject>
#include <QQmlEngine>

#include "source/MainScene.h"
#include "source/helpers/firststartpreparer.h"
#include "source/managers/groupsmanager.h"
#include "source/managers/networkmanager.h"

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
    std::unique_ptr<NetworkManager> mNetwork;

    // Dependent init
    std::unique_ptr<GroupsManager> mGroupsManager;
    std::unique_ptr<MainScene>     mScene;

    NetworkManager* network() const;
    GroupsManager*  groups() const;
    MainScene*      scene() const;

    static bool readonlyMode();
};
