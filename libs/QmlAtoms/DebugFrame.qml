// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick

import Tools

Rectangle {
    id: debugFrame
    anchors.fill: parent
    color: "transparent"

    property point globalPos: mapToGlobal(x, y)
    property string geometry: `${width}x${height}`
    property color baseColor: Debug.randomColor()

    border {
        width: 1
        color: baseColor
    }

    Rectangle {
        opacity: 0.7

        property bool insideFrame: debugFrame.width > (width * 2)

        anchors {
            horizontalCenter: parent.horizontalCenter

            bottom: insideFrame ? parent.bottom : undefined
            top: insideFrame ? undefined : parent.bottom

            bottomMargin: width * 0.1
            topMargin: width * 0.1
        }

        width: sizeText.width
        height: sizeText.height

        color: parent.baseColor
        radius: 3

        Text {
            id: sizeText
            text: debugFrame.geometry

            anchors.bottom: parent.bottom
            anchors.left: parent.left
            padding: contentWidth * 0.03

            color: "white"
        }
    }
//    MouseArea {
//        z: 1000
//        anchors.fill: parent
//        hoverEnabled: true

//        property point pos: Qt.point(mouseX, mouseY)

//        onPosChanged: {
//            const debuggedObject = parent.parent;
//            console.log(debuggedObject)
//            for (let i = 0; i < debuggedObject.children; i++) {

//            }
//        }
//    }
}
