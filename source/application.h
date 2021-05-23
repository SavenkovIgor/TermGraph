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

#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "source/Helpers/appconfig.h"
#include "source/Helpers/firststartpreparer.h"
#include "source/Managers/localdatabasestorage.h"
#include "source/Managers/syncmanager.h"
#include "source/mainscene.h"

// -- Qt6 Migration plan --

// TODO: Remove all "tag" naming
// TODO: Remove setContextProperty stuff
// TODO: Maybe add QProperty somewhere? need think.
// TODO: How to mix QProperty & Q_PROPERTY
// TODO: What about import versions in qml?
// TODO: Replace Components with component properties
// TODO: Create GroupsModel
// TODO: TermDataWrapper to lowercase
// TODO: What is "QQmlEngine::setContextForObject(): Object already has a QQmlContext"
// TODO: What with versioning in my qmldirs?
// TODO: Move getUiElementSize to separate class (maybe singleton+properties)
// TODO: Move all type registration to proper file

class Application : public QObject
{
    Q_OBJECT

public:
    explicit Application(QObject* parent = nullptr);
    ~Application() = default;

    Q_INVOKABLE QString screenshotFilePath(const QString& fileName);

    Q_INVOKABLE int getUiElementSize(const QString& elementTypeName);

private:
    void               initElemSizes();
    QMap<QString, int> elementSizes;

    FirstStartPreparer initPreparer;

    // Independent init
    QScopedPointer<NetworkManager> network;

    // Dependent init
    LocalDatabaseStorage localDb;

    QScopedPointer<NodesManager>          nodesManager;
    QScopedPointer<GroupsManager>         groupsManager;
    QScopedPointer<SyncManager>           syncManager;
    QScopedPointer<MainScene>             scene;
    QScopedPointer<QQmlApplicationEngine> qmlEngine;

    static qreal screenPixelDensity();
};
