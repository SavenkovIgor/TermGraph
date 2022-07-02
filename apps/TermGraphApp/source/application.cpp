// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/application.h"

#include "source/datasourcefactory.h"
#include "source/managers/notificationmanager.h"

Application::Application(QObject* parent)
    : QObject(parent)
    , initPreparer()
    , network(new NetworkManager(NotificationManager::instance()))
    , groupsManager(new GroupsManager(DataSourceFactory::defaultSource(), NotificationManager::instance()))
    , syncManager(new SyncManager(network.get(), groupsManager.get()))
    , scene(new MainScene(groupsManager.get()))
    , qmlEngine(new QQmlApplicationEngine())
{
    groupsManager->updateGroupUuidNameMaps();

    qmlRegisterSingletonInstance("Network", 1, 0, "NetworkManager", network.get());

    qmlRegisterSingletonInstance("Api", 1, 0, "Scene", scene.get());
    qmlRegisterSingletonInstance("Api", 1, 0, "GroupsManager", groupsManager.get());
    qmlRegisterSingletonInstance("Api", 1, 0, "SyncManager", syncManager.get());

    qmlEngine->addImportPath(QStringLiteral("qrc:/"));
    qmlEngine->load(QStringLiteral("qrc:/main.qml"));
}
