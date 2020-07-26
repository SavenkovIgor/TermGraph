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
import QtQuick.Controls 2.15

import StyleInfo 1.0

Drawer {
    id: root

    property color backgroundColor: Colors.baseLight

    edge: Qt.BottomEdge

    topPadding:   Sizes.baseR75
    leftPadding:  Sizes.baseR75
    rightPadding: Sizes.baseR75
    bottomPadding:   Sizes.baseR75

    dim: false

    background: TopRoundedRect {
        PinBar {
            anchors {
                top: parent.top
                topMargin: height / 1.5
                horizontalCenter: parent.horizontalCenter
            }

            width: parent.width * 0.3
        }

        color: root.backgroundColor
    }
}
