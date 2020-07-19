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
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import StyleInfo 1.0
import "../Atoms" as A

Item {
    id: root

    property int size: 200

    width: size
    height: size

    property color col1: "#6d9a28"
    property color col2: Colors.accent
    property color col3: Colors.baseLight

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
