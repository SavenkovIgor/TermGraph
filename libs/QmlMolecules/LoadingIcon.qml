// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Window
import QtQuick.Controls

import Theme
import Atoms as A

Item {
    id: root

    property int size: 200

    width: size
    height: size

    property color col1: "#6d9a28"
    property color col2: Theme.color.accent
    property color col3: Theme.color.base2

    readonly property int animDuration: 1500

    component InfiniteRandomRotation: SequentialAnimation {

        property alias target: rot.target
        running: true
        loops: 1

        onRunningChanged: {
            if (!running) {
                rot.direction = rot.randomDirection();
                start();
            }
        }

        RotationAnimation {
            id: rot
            from: 0
            to: direction === RotationAnimation.Clockwise ? 60 : -60
            duration: root.animDuration
            direction: randomDirection()
            easing.type: Easing.InOutBack

            function coin() { return Math.random() > 0.5; }

            function randomDirection() {
                return coin() ? RotationAnimation.Clockwise : RotationAnimation.Counterclockwise
            }
        }

        PauseAnimation {
            duration: root.animDuration / 3
        }
    }

    A.Hex {
        id: hex1
        anchors.centerIn: parent
        width: root.width
        height: root.height
        color: root.col1

        InfiniteRandomRotation { target: hex1 }
    }

    A.Hex {
        id: hex2
        anchors.centerIn: parent
        width: parent.width * 0.866
        height: parent.height * 0.866
        color: root.col2
        rotation: 30

        InfiniteRandomRotation { target: hex2 }
    }

    A.Hex {
        id: hex3
        anchors.centerIn: parent
        width: parent.width * Math.pow(0.866, 2)
        height: parent.height * Math.pow(0.866, 2)
        color: root.col3
        rotation: 60

        InfiniteRandomRotation { target: hex3 }
    }
}
