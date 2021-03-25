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

import JsTools 1.0

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
