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
#include <QApplication>
#include <QSurface>
#include <QtQuickControls2/QQuickStyle>

#include "source/Helpers/platform.h"

#ifdef Q_OS_ANDROID
#include <QtSvg>  //Because deployment sometimes just forgets to include this lib otherwise
#endif

// TODO: App icon is lost by some reason
// TODO: Differen application binary on android?

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    QQuickStyle::setStyle("Material");  // Important thing for styles!

    // TODO: Check it on mobile!
    if constexpr (Platform::isDesktop()) {
        QSurfaceFormat format = QSurfaceFormat::defaultFormat();
        format.setSamples(8);
        QSurfaceFormat::setDefaultFormat(format);
    }

    MainWindow w;

    return a.exec();
}
