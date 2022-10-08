// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QGuiApplication>
#include <QSurface>
#include <QQmlApplicationEngine>
#include <QtQuickControls2/QQuickStyle>

#include <CommonTools/Platform.h>

// -- Qt6 Migration plan --
// TODO: Use new uuids!

// TODO: Remove all "tag" naming
// TODO: How to mix QProperty & Q_PROPERTY
// TODO: Replace Components with component properties
// TODO: Create GroupsModel
// TODO: What is "QQmlEngine::setContextForObject(): Object already has a QQmlContext"
// TODO: Add namespace to graph
// TODO: Remove get from getters. Helper regexp: \bget.*\(

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

    QQmlApplicationEngine qmlEngine;
    // TODO: On Qt 6.4 this path is imported by default so this line can be deleted
    qmlEngine.addImportPath(QStringLiteral("qrc:/qt/qml"));
    qmlEngine.load(QStringLiteral("qrc:/qt/qml/Api/qml/main.qml"));

    return a.exec();
}
