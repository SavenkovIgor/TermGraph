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

import QtQuick 2.15

import "../Js/Colors.js" as Colors

Rectangle {
    id: root

    color: Colors.base

    required property bool showLoading;

    AppIcon {
        anchors.centerIn: parent

        width: Math.min(root.width, root.height) * 0.1
        height: width
    }

    visible: scene.groupLoading

    onShowLoadingChanged: {
        if (showLoading) {
            timer.start(150);
        } else {
            timer.stop();
            root.visible = false;
        }
    }

    Timer {
        id: timer
        onTriggered: root.visible = true
    }
}
