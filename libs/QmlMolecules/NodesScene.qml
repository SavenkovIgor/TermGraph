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

import QtQuick
import QtQuick.Controls
import QtQuick.Shapes

import Api

import Atoms as A
import StyleInfo

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

    Component { id: edgeComponent; A.Edge { } }

    function createEdge(parent, edgeInfo) {
        return edgeComponent.createObject(parent, edgeInfo);
    }

    function createNullEdge(parent) {
        return createEdge(parent, { pt1: Qt.point(0, 0), pt2: Qt.point(0, 0), isSelected: false });
    }

    function edgeData(index) {
        const modelIndex = Scene.edges.index(index, 0);
        return {
            // Laziness...
            pt1:           Scene.edges.data(modelIndex, 0), // Roles::Pt1
            pt2:           Scene.edges.data(modelIndex, 1), // Roles::Pt2
            isSelected:    Scene.edges.data(modelIndex, 2), // Roles::IsSelected
            edgeType:      Scene.edges.data(modelIndex, 3), // Roles::EdgeType
            edgeSelection: Scene.edges.data(modelIndex, 4)  // Roles::EdgeSelection
        }
    }

    function edgesData() {
        const ret = []
        const edgesCount = Scene.edges.rowCount();

        for (let j = 0; j < edgesCount; ++j)
            ret.push(root.edgeData(j))

        return ret;
    }

    contentItem: Item {

        Rectangle {
            id: graphFrame
            anchors.fill: parent
            anchors.margins: 35

            color: Scene.hasSelection ? "#BB000000" : "transparent"
            border { color: Colors.white; width: 2 }
            visible: Scene.hasCurrentGroup
            radius: 10
            z: 3

            Text {
                anchors { left: parent.left; top: parent.top; }
                topPadding: 15
                leftPadding: 20
                text: Scene.hasCurrentGroup ? Scene.currentGroup.name : ""
                color: Colors.white
                font: Fonts.setWeight(Fonts.term, Font.DemiBold)
            }
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

            function clear() {
                const len = data.length;
                for (let i = 0; i < len; ++i)
                    data[i].destroy();

                data = createNullEdge(edgesShape);
            }

            function updateEdges() {
                clear();
                const notSelected = edgesData().filter(e => !e.isSelected);

                if (notSelected.length !== 0)
                    data = notSelected.map(e => root.createEdge(edgesShape, e));
            }
        }

        Shape {
            id: edgesSelectedShape
            z: 4
            visible: Scene.hasSelection

            function clear() {
                const len = data.length;
                for (let i = 0; i < len; ++i)
                    data[i].destroy();

                data = createNullEdge(edgesSelectedShape);
            }

            function updateEdges() {
                clear();
                const selected = edgesData().filter(e => e.isSelected);

                if (selected.length !== 0)
                    data = selected.map(e => root.createEdge(edgesSelectedShape, e));
            }
        }

        Repeater {
            model: Scene.terms

            delegate: A.Term {
                rect: model.rect
                radius: model.radius
                type: model.type
                isSelected: model.isSelected
                weight: model.weight
                text: model.term
                z: model.isSelected ? 5 : 2
            }
        }
    }
}
