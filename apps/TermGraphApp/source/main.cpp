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

#include <QApplication>
#include <QDir>
#include <QSurface>
#include <QtQuickControls2/QQuickStyle>
#include <QtQml/qqmlextensionplugin.h>

#include "source/application.h"
#include "source/helpers/platform.h"

#include "source/qmlTypesRegistrator.h"

#ifdef Q_OS_ANDROID
#include <QtSvg> //Because deployment sometimes just forgets to include this lib otherwise
#endif

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

Q_IMPORT_QML_PLUGIN(ToolsPlugin);
Q_IMPORT_QML_PLUGIN(StyleInfoPlugin);
Q_IMPORT_QML_PLUGIN(AtomsPlugin);
Q_IMPORT_QML_PLUGIN(MoleculesPlugin);
Q_IMPORT_QML_PLUGIN(PagesPlugin);

void setSurfaceFormat()
{
    // This is need for good aliasing of term edges on qml scene
    // for desktop & mobile
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setSamples(Platform::isDesktop() ? 8 : 4);
    QSurfaceFormat::setDefaultFormat(format);
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    Q_INIT_RESOURCE(SqlQueries);

    QQuickStyle::setStyle("Material"); // Important thing for styles!

    setSurfaceFormat();

    Application w;

    return a.exec();
}
