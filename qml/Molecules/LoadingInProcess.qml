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

import StyleInfo 1.0

Rectangle {
    id: root

    color: Colors.base

    readonly property int animDuration: 250
    required property bool showLoading;

    onShowLoadingChanged: {
        if (showLoading) {
            timer.start(200);
        } else {
            timer.stop();
            if (state == "visible")
                state = "hidden"
        }
    }

    Timer {
        id: timer
        onTriggered: root.state = "visible"
    }

    state: "hidden"

    visible: false
    opacity: 0

    states: [
        State { name: "visible" },
        State { name: "hidden" }
    ]

    transitions: [
        Transition {
            from: "*"; to: "visible";

            SequentialAnimation {
                PropertyAction { target: root; property: "opacity"; value: 0; }
                PropertyAction { target: root; property: "visible"; value: true; }
                OpacityAnimator { target: root; from: 0; to: 1; duration: root.animDuration; }
            }
        },

        Transition {
            from: "*"; to: "hidden";

            SequentialAnimation {
                OpacityAnimator { target: root; from: 1; to: 0; duration: root.animDuration; }
                PropertyAction { target: root; property: "opacity"; value: 0; }
                PropertyAction { target: root; property: "visible"; value: false; }
            }
        }
    ]

    LoadingIcon {
        anchors.centerIn: parent
        size: Math.min(root.width, root.height) * 0.15
    }
}
