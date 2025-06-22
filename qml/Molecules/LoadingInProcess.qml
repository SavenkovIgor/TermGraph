// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick

import Theme

Rectangle {
    id: root

    color: Theme.color.base0

    readonly property int animDuration: 250
    required property bool showLoading

    onShowLoadingChanged: {
        if (showLoading) {
            timer.start();
        } else {
            timer.stop();
            if (state == "visible")
                state = "hidden"
        }
    }

    Timer {
        id: timer
        interval: 200
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
