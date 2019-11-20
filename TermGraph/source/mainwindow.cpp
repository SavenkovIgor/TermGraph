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

#include "mainwindow.h"

#include <QScreen>

#include "source/Helpers/platform.h"

MainWindow::MainWindow(QObject *parent):
    QObject(parent),
    network(new NetworkManager()),
    nodesManager(new NodesManager()),
    paintManager(new PaintManager()),
    analyze(new WordFreqAnalyze()),
    tagProcessor(new TagProcessor()),
    textProcessor(new TextProcessor()),
    groupsManager(new GroupsManager(nodesManager.get())),
    syncManager(new SyncManager(network.get(), groupsManager.get(), nodesManager.get())),
    scene(new MainScene(groupsManager.get(), nodesManager.get(), paintManager.get())),
    qmlEngine(new QQmlApplicationEngine())
{
    initElemSizes();
    AppSettings::StdPaths::createDefaultFoldersIfNeed();

    QQuickStyle::setStyle("Material");

    // Database init
    groupsManager->updateGroupUuidNameMaps();

    // remind = new Reminder(scene->getAllNodes());

    auto registration = []([[maybe_unused]] QQmlEngine* engine,
                           [[maybe_unused]] QJSEngine*  scriptEngine) -> QObject* {
        return &NotificationManager::instance();
    };

    qmlRegisterSingletonType<NotificationManager>("Notification", 1, 0, "Notification", registration);

    qmlRegisterType<PaintedTerm>();

    qmlEngine->rootContext()->setContextProperty("mainObj", this);
    qmlEngine->rootContext()->setContextProperty("sceneObj", scene.get());
    qmlEngine->rootContext()->setContextProperty("paintManager", paintManager.get());
    qmlEngine->rootContext()->setContextProperty("networkManager", network.get());
    qmlEngine->rootContext()->setContextProperty("groupsManager", groupsManager.get());
    qmlEngine->rootContext()->setContextProperty("nodesManager", nodesManager.get());
    qmlEngine->rootContext()->setContextProperty("tagProcessor", tagProcessor.get());
    qmlEngine->rootContext()->setContextProperty("textProcessor", textProcessor.get());
    qmlEngine->rootContext()->setContextProperty("syncManager", syncManager.get());
    qmlEngine->load(QUrl("qrc:/qml/MainWindow.qml"));
}

QString MainWindow::screenshotNameAndPath(const QString& groupUuid)
{
    assert(!groupUuid.isEmpty());

    QStringList checkPaths;

    checkPaths << QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    checkPaths << QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    checkPaths << QStandardPaths::standardLocations(QStandardPaths::DownloadLocation);

    auto fileName = groupUuid + ".png";

    for (const auto& path : checkPaths) {
        auto fullPath = path + "/" + fileName;
        if (FSWorks::createFile(fullPath)) {
            // If we can create such file we remove it and return path
            FSWorks::removeFile(fullPath);
            return fullPath;
        }
    }

    NotificationManager::showError("Директория для записи не найдена");
    return "";
}

int MainWindow::getUiElementSize(const QString &elementTypeName)
{
    if (elementSizes.contains(elementTypeName)) {
        // чтобы эти значения не вставлялись. на всякий случай
        auto screenDencity = MainWindow::screenPixelDensity();
        auto screenCorrectedSize = screenDencity * elementSizes[elementTypeName];
        return static_cast<int>(screenCorrectedSize);
    }
    qDebug() << "Отсутствует размер для элемента:" << elementTypeName;
    return 0;
}

int MainWindow::dbVersion()
{
    return Database::instance().appConfigTable->getDbVersion();
}

void MainWindow::initElemSizes()
{
    if constexpr (Platform::isDesktop())
    {
        // Screen.pixelDencity - pixel in millimeters
        elementSizes["roundButton"] = 15;
        elementSizes["text"]        = 5;
        elementSizes["capitalText"] = 6;
        elementSizes["inputLabel"]  = 7;
        elementSizes["inputText"]   = 6;
        elementSizes["combo"]       = 5;
        elementSizes["button"]      = 5;
        elementSizes["colSpace"]    = 4;
        elementSizes["appHeader"]   = 9;
        elementSizes["infoLabel"]   = 6;
        elementSizes["sideMenu"]    = 7;
    }

    if constexpr (Platform::isAndroid())
    {
        elementSizes["roundButton"] = 9;
        elementSizes["text"]        = 4;
        elementSizes["capitalText"] = 5;
        elementSizes["inputLabel"]  = 4;
        elementSizes["inputText"]   = 4;
        elementSizes["combo"]       = 4;
        elementSizes["button"]      = 4;
        elementSizes["colSpace"]    = 2;
        elementSizes["appHeader"]   = 6;
        elementSizes["infoLabel"]   = 4;
        elementSizes["sideMenu"]    = 5;
    }
}

qreal MainWindow::screenPixelDensity()
{
    const qreal inchToMillimeterRatio = 0.039370;

    auto dotsPerInch        = qApp->primaryScreen()->physicalDotsPerInch();
    auto dontsPerMillimeter = dotsPerInch * inchToMillimeterRatio;

    return dontsPerMillimeter;
}
