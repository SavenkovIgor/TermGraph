/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

#include "source/application.h"

Application::Application(QObject* parent)
    : QObject(parent)
    , initPreparer()
    , network(new NetworkManager(NotificationManager::instance()))
    , localDb(AppSettings::StdPaths::defaultDatabaseFilePath(), AppSettings::StdPaths::backupFolder())
    , groupsManager(new GroupsManager(localDb, NotificationManager::instance()))
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
