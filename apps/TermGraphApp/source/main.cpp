// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QDir>
#include <QGuiApplication>
#include <QSurface>
#include <QtQml/qqmlextensionplugin.h>
#include <QtQuickControls2/QQuickStyle>

#include <CommonTools/Platform.h>

#include "source/application.h"
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
Q_IMPORT_QML_PLUGIN(StyleInfoPlugin);
Q_IMPORT_QML_PLUGIN(AtomsPlugin);
Q_IMPORT_QML_PLUGIN(MoleculesPlugin);
Q_IMPORT_QML_PLUGIN(PagesPlugin);

void initResources()
{
#ifndef Q_OS_WASM
    Q_INIT_RESOURCE(SqlQueries);
#endif
    Q_INIT_RESOURCE(HelpImages);
    Q_INIT_RESOURCE(Icons);
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

    Application w;

    return a.exec();
}
