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

#include <QApplication>
#include <QScreen>

#include "source/Helpers/platform.h"
#include "source/Managers/notificationmanager.h"
#include "source/Model/Termin/paintedterm.h"
#include "source/Model/TerminEdge/edge.h"

Application::Application(QObject* parent)
    : QObject(parent)
    , initPreparer()
    , network(new NetworkManager())
    , localDb()
    , nodesManager(new NodesManager(localDb))
    , groupsManager(new GroupsManager(localDb, nodesManager.get()))
    , syncManager(new SyncManager(network.get(), groupsManager.get(), nodesManager.get()))
    , scene(new MainScene(groupsManager.get(), nodesManager.get()))
    , qmlEngine(new QQmlApplicationEngine())
{
    initElemSizes();

    groupsManager->updateGroupUuidNameMaps();

    // remind = new Reminder(scene->getAllNodes());

    qmlRegisterAnonymousType<PaintedTerm>("Graph", 1);
    qmlRegisterAnonymousType<Edge>("Graph", 1);

    qmlEngine->rootContext()->setContextProperty("mainObj", this);
    qmlEngine->rootContext()->setContextProperty("scene", scene.get());
    qmlEngine->rootContext()->setContextProperty("networkManager", network.get());
    qmlEngine->rootContext()->setContextProperty("groupsManager", groupsManager.get());
    qmlEngine->rootContext()->setContextProperty("nodesManager", nodesManager.get());
    qmlEngine->rootContext()->setContextProperty("syncManager", syncManager.get());

    qmlEngine->addImportPath(QStringLiteral("qrc:/"));
    qmlEngine->load(QStringLiteral("qrc:/main.qml"));
}

QString Application::screenshotFilePath(const QString& fileName)
{
    assert(!fileName.isEmpty());

    QStringList checkPaths;

    checkPaths << QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    checkPaths << QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    checkPaths << QStandardPaths::standardLocations(QStandardPaths::DownloadLocation);

    for (const auto& path : checkPaths) {
        auto fullPath = path + "/" + fileName + ".png";
        if (FSWorks::createFile(fullPath)) {
            // If we can create such file we remove it and return path
            FSWorks::deleteFile(fullPath);
            return fullPath;
        }
    }

    NotificationManager::showError("Директория для записи не найдена");
    return "";
}

int Application::getUiElementSize(const QString& elementTypeName)
{
    if (elementSizes.contains(elementTypeName)) {
        // чтобы эти значения не вставлялись. на всякий случай
        auto screenDencity       = Application::screenPixelDensity();
        auto screenCorrectedSize = screenDencity * elementSizes[elementTypeName];
        return static_cast<int>(screenCorrectedSize);
    }

    NotificationManager::showDebug(QString("Отсутствует размер для элемента: %1").arg(elementTypeName));
    return 0;
}

int Application::dbVersion() { return localDb.storageVersion(); }

void Application::initElemSizes()
{
    if constexpr (Platform::isDesktop()) {
        // Screen.pixelDencity - pixel in millimeters
        elementSizes["roundButton"] = 15;
        elementSizes["text"]        = 5;
        elementSizes["capitalText"] = 6;
        elementSizes["inputLabel"]  = 7;
        elementSizes["inputText"]   = 6;
        elementSizes["button"]      = 5;
        elementSizes["colSpace"]    = 4;
        elementSizes["appHeader"]   = 9;
    }

    if constexpr (Platform::isAndroid()) {
        elementSizes["roundButton"] = 9;
        elementSizes["text"]        = 4;
        elementSizes["capitalText"] = 5;
        elementSizes["inputLabel"]  = 4;
        elementSizes["inputText"]   = 4;
        elementSizes["button"]      = 4;
        elementSizes["colSpace"]    = 2;
        elementSizes["appHeader"]   = 6;
    }
}

qreal Application::screenPixelDensity()
{
    const qreal inchToMillimeterRatio = 0.039370;

    auto dotsPerInch        = qApp->primaryScreen()->physicalDotsPerInch();
    auto dontsPerMillimeter = dotsPerInch * inchToMillimeterRatio;

    return dontsPerMillimeter;
}
