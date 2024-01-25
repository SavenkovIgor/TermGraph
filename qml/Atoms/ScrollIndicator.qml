// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls as C

import Theme

C.ScrollIndicator {
    id: root

    contentItem: RoundedRectangle {
        id: contentRect

        property color baseColor: "#77e8e8e8"

        implicitWidth:  root.vertical   ? Sizes.rm : 100
        implicitHeight: root.horizontal ? Sizes.rm : 100
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
