// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick

Rectangle {
    id: root

    property real fps: 0
    property int frameCount: 0
    property real lastTime: 0
    property real updateInterval: 1000 // Update every 1000ms (1 second)

    width: fpsText.implicitWidth + 16
    height: fpsText.implicitHeight + 8
    color: "#3f3e40" // Theme.color.base1
    border.color: "#79797b" // Theme.color.base3
    border.width: 1
    radius: 4
    opacity: 0.8

    Text {
        id: fpsText
        anchors.centerIn: parent
        text: "FPS: " + root.fps.toFixed(1)
        color: "#ffffff" // Theme.color.base7
        font.pixelSize: 12
    }

    Timer {
        id: fpsTimer
        interval: 16 // ~60 FPS update rate for smooth counting
        running: true
        repeat: true

        onTriggered: {
            root.frameCount++
            var currentTime = Date.now()

            if (root.lastTime === 0) {
                root.lastTime = currentTime
                return
            }

            var timeDiff = currentTime - root.lastTime

            if (timeDiff >= root.updateInterval) {
                root.fps = (root.frameCount * 1000) / timeDiff
                root.frameCount = 0
                root.lastTime = currentTime
            }
        }
    }
}
