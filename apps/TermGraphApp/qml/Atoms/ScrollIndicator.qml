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

import QtQuick
import QtQuick.Controls

import StyleInfo

ScrollIndicator {
    id: root

    contentItem: RoundedRectangle {
        id: contentRect

        property color baseColor: "#77e8e8e8"

        implicitWidth:  root.vertical   ? Sizes.baseR75 / 3 : 100
        implicitHeight: root.horizontal ? Sizes.baseR75 / 3 : 100
        color: "transparent"

        state: root.active ? "visible" : "hidden"

        states: [
            State {
                name: "visible"
                PropertyChanges {
                    target: contentRect
                    color: contentRect.baseColor
                }
            },
            State { name: "hidden" }
        ]

        transitions: [
            Transition {
                from: "visible"
                to: "hidden"

                SequentialAnimation {
                    PauseAnimation { duration: 500 }
                    ColorAnimation { from: contentRect.baseColor; to: "transparent"; duration: 500; }
                }
            },
            Transition {
                from: "hidden"
                to: "visible"

                ColorAnimation { from: "transparent"; to: contentRect.baseColor; duration: 200; }
            }

        ]
    }
}
