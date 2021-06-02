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
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15

import Api 1.0

import Atoms 1.0 as A
import StyleInfo 1.0

Control {
    id: root

    height: Scene.sceneRect.height
    width: Scene.sceneRect.width

    Component.onCompleted: {
        // Try to show first group
        const groupsUuids = GroupsManager.allUuidSorted;

        if (groupsUuids.length !== 0) {
            Scene.selectGroup(groupsUuids[0]);
        }
    }

    function makeScreenshot() {
        grabToImage(function(result){
            const groupName = Scene.currentGroup.name;
            const name = Application.screenshotFilePath(groupName);
            if (name !== "") {
                result.saveToFile(name);
                Notification.showInfo("Снимок группы создан. Путь:" + name);
            } else {
                Notification.showInfo("Снимок не создан((");
            }
        });
    }

    Component { id: edgeComponent; A.Edge { } }

    function createEdge(parent, edgeInfo) {
        return edgeComponent.createObject(parent,
                                          { pt1: edgeInfo.pt1, pt2: edgeInfo.pt2, edgeColor: edgeInfo.color });
    }

    function createNullEdge(parent) {
        return createEdge(parent, { pt1: Qt.point(0, 0), pt2: Qt.point(0, 0), color: "black" });
    }

    contentItem: Item {

        Rectangle {
            id: graphFrame
            anchors.fill: parent
            anchors.margins: 35

            color: "transparent"
            border { color: Colors.white; width: 2 }
            visible: Scene.hasCurrentGroup
            radius: 10
            z: 6

            Text {
                anchors { left: parent.left; top: parent.top; }
                topPadding: 15
                leftPadding: 20
                text: Scene.hasCurrentGroup ? Scene.currentGroup.name : ""
                color: Colors.white
                font: Fonts.setWeight(Fonts.term, Font.DemiBold)
            }
        }

        Rectangle {  // Selection background
            anchors.fill: graphFrame
            visible: Scene.hasSelection

            z: 3
            color: "#BB000000"
            radius: graphFrame.radius
        }

        Connections {
            target: Scene

            function onEdgesChanged() {
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

                let newEdges = [root.createNullEdge(edgesShape)];

                const edgesCount = Scene.edges.length;

                for (let j = 0; j < edgesCount; ++j) {
                    const edgeLine = root.createEdge(edgesShape, Scene.edges[j]);

                    newEdges.push(edgeLine);
                }

                edgesShape.data = newEdges;
            }
        }

        Shape {
            id: edgesSelectedShape
            z: 4
            visible: Scene.hasSelection

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

                let newEdges = [root.createNullEdge(edgesSelectedShape)];

                const edgesCount = Scene.edges.length;

                for (let j = 0; j < edgesCount; ++j) {

                    const edge = Scene.edges[j];

                    if (!edge.isSelected)
                        continue;

                    const edgeLine = root.createEdge(edgesSelectedShape, edge);

                    newEdges.push(edgeLine);
                }

                edgesSelectedShape.data = newEdges;
            }
        }

        Repeater {
            model: Scene.terms

            delegate: A.Term {
                rect: model.rect
                radius: model.radius
                color: model.color
                weight: model.weight
                text: model.term
                z: model.isSelected ? 5 : 2
            }
        }
    }
}
