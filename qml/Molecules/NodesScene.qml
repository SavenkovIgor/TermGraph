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

import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Shapes 1.14

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

    Component { id: edgeComponent; A.Edge { } }

    contentItem: Item {

        Rectangle {
            id: graphFrame
            anchors.fill: parent
            anchors.margins: 35

            color: "transparent"
            border { color: Colors.white; width: 2 }
            radius: 10
            z: 6

            Text {
                anchors { left: parent.left; top: parent.top; }
                topPadding: 15
                leftPadding: 20
                text: scene.currentGroupName
                color: Colors.white
                font: Fonts.setWeight(Fonts.term, Font.DemiBold)
            }
        }

        Rectangle {  // Selection background
            anchors.fill: graphFrame
            visible: scene.hasSelection

            z: 3
            color: "#BB000000"
            radius: graphFrame.radius
        }

        Connections {
            target: scene
            onEdgesChanged: {
                edgesShape.updateEdges();
                edgesSelectedShape.updateEdges();
            }
        }

        Shape {
            id: edgesShape
            z: 1

            function updateEdges() {
                // Delete old edges
                const delArrayLength = edgesShape.data.length;

                for (let i = 0; i < delArrayLength; ++i)
                    edgesShape.data[i].destroy();

                // Create new edges

                // Looks like a bug in shapes.
                // If shapes data is empty, it wouldn't be redrawed
                // It means that, if we remove all edges at once,
                // old edges wouldn't disappear, and would be still visible
                // So we create fake edge every time just for sure
                // that shape redraw each time

                let fakeEdge = edgeComponent.createObject(
                        edgesShape,
                        {
                            pt1: Qt.point(0, 0),
                            pt2: Qt.point(0, 0),
                            edgeColor: "black"
                        });

                let newEdges = [fakeEdge];

                const edgesCount = scene.edges.length;

                for (let j = 0; j < edgesCount; ++j) {

                    const edge = scene.edges[j];

                    const edgeLine = edgeComponent.createObject(
                            edgesShape,
                            {
                                pt1: edge.pt1,
                                pt2: edge.pt2,
                                edgeColor: edge.color
                            });

                    newEdges.push(edgeLine);
                }

                edgesShape.data = newEdges;
            }
        }

        Shape {
            id: edgesSelectedShape
            z: 4
            visible: scene.hasSelection

            function updateEdges() {
                // Delete old edges
                const delArrayLength = edgesSelectedShape.data.length;

                for (let i = 0; i < delArrayLength; ++i)
                    edgesSelectedShape.data[i].destroy();

                // Create new edges

                // Looks like a bug in shapes.
                // If shapes data is empty, it wouldn't be redrawed
                // It means that, if we remove all edges at once,
                // old edges wouldn't disappear, and would be still visible
                // So we create fake edge every time just for sure
                // that shape redraw each time

                let fakeEdge = edgeComponent.createObject(
                        edgesSelectedShape,
                        {
                            pt1: Qt.point(0, 0),
                            pt2: Qt.point(0, 0),
                            edgeColor: "black"
                        });

                let newEdges = [fakeEdge];

                const edgesCount = scene.edges.length;

                for (let j = 0; j < edgesCount; ++j) {

                    const edge = scene.edges[j];

                    if (!edge.isSelected)
                        continue;

                    const edgeLine = edgeComponent.createObject(
                            edgesSelectedShape,
                            {
                                pt1: edge.pt1,
                                pt2: edge.pt2,
                                edgeColor: edge.color
                            });

                    newEdges.push(edgeLine);
                }

                edgesSelectedShape.data = newEdges;
            }
        }

        Repeater {
            model: scene.nodes

            delegate: A.Term {
                rect: modelData.rect
                radius: modelData.radius
                color: modelData.color
                text: modelData.term
                z: modelData.isSelectedAnyway ? 5 : 2
            }
        }
    }
}
