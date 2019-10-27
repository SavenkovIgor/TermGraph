/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
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
#include <QtQuickControls2/QQuickStyle>

#include "source/Model/reminder.h"
#include "source/mainscene.h"
#include "source/paintqueuemanager.h"
#include "source/databaseWorks/database.h"
#include "source/Model/wordfreqanalyze.h"
#include "source/Helpers/tagprocessor.h"
#include "source/Helpers/textprocessor.h"
#include "source/Helpers/appconfig.h"
#include "source/Managers/syncmanager.h"
#include "source/Managers/notificationmanager.h"

class MainWindow : public QObject
{
    Q_OBJECT

public:
    explicit MainWindow(QObject *parent = nullptr);
    ~MainWindow() = default;

    Q_INVOKABLE QString screenshotNameAndPath(const QString& groupUuid);

public slots:
    int getUiElementSize(const QString& elementTypeName);
    int dbVersion();

private:
    void initElemSizes();
    QMap<QString, int> elementSizes;

    // Independent init
    QScopedPointer<NetworkManager> network;
    QScopedPointer<NodesManager> nodesManager;
    QScopedPointer<PaintManager> paintManager;

    // Helper stuff (still independent init)
    QScopedPointer<WordFreqAnalyze> analyze;
    QScopedPointer<TagProcessor> tagProcessor;
    QScopedPointer<TextProcessor> textProcessor;

    // Dependent init
    QScopedPointer<GroupsManager> groupsManager;
    QScopedPointer<SyncManager> syncManager;
    QScopedPointer<MainScene> scene;
    QScopedPointer<QQmlApplicationEngine> qmlEngine;

    static qreal screenPixelDensity();
};
