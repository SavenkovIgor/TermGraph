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
#include "source/Helpers/textprocessor.h"
#include "source/Managers/localdatabasestorage.h"
#include "source/Managers/syncmanager.h"
#include "source/mainscene.h"

class Application : public QObject
{
    Q_OBJECT

public:
    explicit Application(QObject* parent = nullptr);
    ~Application() = default;

    Q_INVOKABLE QString screenshotFilePath(const QString& fileName);

public slots:
    int getUiElementSize(const QString& elementTypeName);
    int dbVersion();

private:
    void               initElemSizes();
    QMap<QString, int> elementSizes;

    FirstStartPreparer initPreparer;

    // Independent init
    QScopedPointer<NetworkManager> network;

    // Helper stuff (still independent init)
    QScopedPointer<TextProcessor> textProcessor;

    // Dependent init
    LocalDatabaseStorage localDb;

    QScopedPointer<NodesManager>          nodesManager;
    QScopedPointer<GroupsManager>         groupsManager;
    QScopedPointer<SyncManager>           syncManager;
    QScopedPointer<MainScene>             scene;
    QScopedPointer<QQmlApplicationEngine> qmlEngine;

    static qreal screenPixelDensity();
};
