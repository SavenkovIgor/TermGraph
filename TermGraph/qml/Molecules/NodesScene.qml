/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
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

import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Shapes 1.13

import "../Atoms" as A

import "../Js/Fonts.js" as Fonts
import "../Js/Colors.js" as Colors

import Notification 1.0

Control {

    height: scene.sceneRect.height
    width: scene.sceneRect.width

    background: Rectangle { color: Colors.base }

    Component.onCompleted: {
        // Try to show first group
        const groupsUuids = groupsManager.allUuidSorted;

        if (groupsUuids.length !== 0) {
            scene.currentGroup = groupsUuids[0];
        }
    }

    function makeScreenshot() {
        grabToImage(function(result){
            const groupName = scene.currentGroupName;
            const name = mainObj.screenshotFilePath(groupName);
            if (name !== "") {
                result.saveToFile(name);
                Notification.showInfo("Снимок группы создан. Путь:" + name);
            } else {
                Notification.showInfo("Снимок не создан((");
            }
        });
    }

    contentItem: Item {

        Rectangle {
            readonly property int offset: 35

            anchors {
                fill: parent
                leftMargin: offset
                rightMargin: offset
                topMargin: offset
                bottomMargin: offset
            }

            color: "transparent"
            border { color: Colors.white; width: 2 }
            radius: 10

            Text {
                anchors { left: parent.left; top: parent.top; }
                topPadding: 15
                leftPadding: 20
                text: scene.currentGroupName
                color: Colors.white
                font: Fonts.setWeight(Fonts.term, Font.DemiBold)
            }
        }

        Connections {
            target: scene
            onEdgesChanged: edgesShape.updateEdges()
        }

        Shape {
            id: edgesShape

            function updateEdges() {
                // Delete old edges
                const delArrayLength = edgesShape.data.length;

                for (let i = 0; i < delArrayLength; ++i)
                    edgesShape.data[i].destroy();

                let newEdges = [];

                const edgeArrayLength = scene.edges.length;

                for (let j = 0; j < edgeArrayLength; ++j) {

                    const pt1 = scene.edges[j].pt1; // paintManager.currentFirstEdgePoint();
                    const pt2 = scene.edges[j].pt2; //paintManager.currentLastEdgePoint();
                    const col = scene.edges[j].color; //paintManager.getEdgeColor();

                    let edge = edgeComponent.createObject(edgesShape, { pt1: pt1, pt2: pt2, edgeColor: col });

                    newEdges.push(edge);

//                    paintManager.nextEdge();
                }

                edgesShape.data = newEdges;
            }
        }

        Component {
            id: edgeComponent

            ShapePath {
                id: sPath

                property point pt1: Qt.point(0, 0)
                property point pt2: Qt.point(0, 0)
                property color edgeColor: "black"
                property real halfWidth: (pt2.x - pt1.x) / 2;
                property point b1: Qt.point(pt1.x + halfWidth, pt1.y)
                property point b2: Qt.point(pt2.x - halfWidth, pt2.y)

                startX: pt1.x; startY: pt1.y;

                strokeWidth: Qt.platform.os === "Android" ? 3 : 2
                strokeColor: edgeColor
                fillColor: "transparent"

                PathCubic {
                    x: sPath.pt2.x; y: sPath.pt2.y;
                    control1X: b1.x; control1Y: b1.y;
                    control2X: b2.x; control2Y: b2.y;
                }
            }
        }

        Repeater {
            model: scene.nodes

            delegate: A.Term {
                rect: modelData.rect
                radius: modelData.radius
                color: modelData.color
                text: modelData.term
            }
        }
    }
}
