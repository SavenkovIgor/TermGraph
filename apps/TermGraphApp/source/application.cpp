// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/application.h"

#include "source/managers/notificationmanager.h"

Application::Application(QObject* parent)
    : QObject(parent)
    , initPreparer()
    , network(new NetworkManager(NotificationManager::instance()))
    , groupsManager(new GroupsManager(NotificationManager::instance()))
    , syncManager(new SyncManager(network.get(), groupsManager.get()))
    , scene(new MainScene(groupsManager.get()))
    , qmlEngine(new QQmlApplicationEngine())
{
    qmlRegisterSingletonInstance("Network", 1, 0, "NetworkManager", network.get());

    qmlRegisterSingletonInstance("Api", 1, 0, "Scene", scene.get());
    qmlRegisterSingletonInstance("Api", 1, 0, "GroupsManager", groupsManager.get());
    qmlRegisterSingletonInstance("Api", 1, 0, "SyncManager", syncManager.get());

    // TODO: On Qt 6.4 this path is imported by default so this line can be deleted
    qmlEngine->addImportPath(QStringLiteral("qrc:/qt/qml"));
    qmlEngine->load(QStringLiteral("qrc:/qt/qml/main.qml"));
}
