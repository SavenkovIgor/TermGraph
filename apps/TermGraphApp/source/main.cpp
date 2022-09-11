// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QDir>
#include <QGuiApplication>
#include <QSurface>
#include <QtQml/qqmlextensionplugin.h>
#include <QQmlApplicationEngine>
#include <QtQuickControls2/QQuickStyle>

#include <CommonTools/Platform.h>

#include "source/api.h"
#include "source/qmlTypesRegistrator.h"

// -- Qt6 Migration plan --
// TODO: Use new uuids!

// TODO: Remove all "tag" naming
// TODO: How to mix QProperty & Q_PROPERTY
// TODO: Replace Components with component properties
// TODO: Create GroupsModel
// TODO: TermDataWrapper to lowercase
// TODO: What is "QQmlEngine::setContextForObject(): Object already has a QQmlContext"
// TODO: Add namespace to graph
// TODO: Add Notifications Interface
// TODO: Remove get from getters. Helper regexp: \bget.*\(
// TODO: Try to cleanup approach. All c++ objects created from qml

Q_IMPORT_QML_PLUGIN(ToolsPlugin);
Q_IMPORT_QML_PLUGIN(ThemePlugin);
Q_IMPORT_QML_PLUGIN(AtomsPlugin);
Q_IMPORT_QML_PLUGIN(MoleculesPlugin);
Q_IMPORT_QML_PLUGIN(PagesPlugin);

void initResources()
{
#ifndef Q_OS_WASM
    Q_INIT_RESOURCE(SqlQueries);
#endif
    Q_INIT_RESOURCE(HelpImages);
}

void setSurfaceFormat()
{
    // This is need for good aliasing of term edges on qml scene
    // for desktop & mobile
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setSamples(Platform::isDesktop() ? -1 : 4);
    QSurfaceFormat::setDefaultFormat(format);
}

int main(int argc, char* argv[])
{
    qSetMessagePattern("%{if-debug}Debug%{endif}"
                       "%{if-info}Info %{endif}"
                       "%{if-warning}Warn %{endif}"
                       "%{if-critical}Crit %{endif}"
                       "%{if-fatal}Fatal%{endif}"
                       "[%{threadid}]: %{message}");

    QGuiApplication a(argc, argv);

    initResources();

    QQuickStyle::setStyle("Material"); // Important thing for styles!

    setSurfaceFormat();

    Api w;

    QQmlApplicationEngine qmlEngine;
    // TODO: On Qt 6.4 this path is imported by default so this line can be deleted
    qmlEngine.addImportPath(QStringLiteral("qrc:/qt/qml"));
    qmlEngine.load(QStringLiteral("qrc:/qt/qml/main.qml"));

    return a.exec();
}
