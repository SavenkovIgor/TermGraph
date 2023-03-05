// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Shapes

import Api

import Atoms as A
import Theme

Control {
    id: root

    height: Api.scene.sceneRect.height
    width: Api.scene.sceneRect.width

    Component.onCompleted: {
        // Try to show first group
        const groupsUuids = Api.groups.allUuidSorted;

        if (groupsUuids.length !== 0) {
            const urlGroupName = groupNameFromUrl(Api.net.currentUrl);

            let uuid = Api.groups.getGroupUuid(urlGroupName);

            if (!uuid)
                uuid = Api.groups.getGroupUuid("Global");

            if (!uuid)
                uuid = groupsUuids[0];

            Api.scene.selectGroup(uuid);
        }
    }

    function groupNameFromUrl(url) {
        const params = new URL(url).searchParams;
        return params.get("group");
    }

    Component { id: edgeComponent; A.Edge { } }

    function createEdge(parent, edgeInfo) {
        return edgeComponent.createObject(parent, edgeInfo);
    }

    function createNullEdge(parent) {
        return createEdge(parent, { pt1: Qt.point(0, 0), pt2: Qt.point(0, 0), isSelected: false });
    }

    function edgeData(index) {
        const modelIndex = Api.scene.edges.index(index, 0);
        return {
            // Laziness...
            pt1:           Api.scene.edges.data(modelIndex, 0), // Roles::Pt1
            pt2:           Api.scene.edges.data(modelIndex, 1), // Roles::Pt2
            isSelected:    Api.scene.edges.data(modelIndex, 2), // Roles::IsSelected
            edgeType:      Api.scene.edges.data(modelIndex, 3), // Roles::EdgeType
            edgeSelection: Api.scene.edges.data(modelIndex, 4)  // Roles::EdgeSelection
        }
    }

    function edgesData() {
        const ret = []
        const edgesCount = Api.scene.edges.rowCount();

        for (let j = 0; j < edgesCount; ++j)
            ret.push(root.edgeData(j))

        return ret;
    }

    contentItem: Item {

        Rectangle {
            id: graphFrame
            anchors.fill: parent
            anchors.margins: 35

            color: Api.scene.hasSelection ? "#BB000000" : "transparent"
            border { color: Theme.color.base6; width: 2 }
            visible: Api.scene.hasCurrentGroup
            radius: 10
            z: 3

            Text {
                anchors { left: parent.left; top: parent.top; }
                topPadding: 15
                leftPadding: 20
                text: Api.scene.hasCurrentGroup ? Api.scene.currentGroup.name : ""
                color: Theme.color.base7
                font: Theme.font.setWeight(Theme.font.term, Font.DemiBold)
            }
        }

        Connections {
            target: Api.scene

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
            visible: Api.scene.hasSelection

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
            model: Api.scene.terms

            delegate: A.Term {
                rect: model.rect
                radius: model.radius
                type: model.type
                isSelected: model.isSelected
                weight: model.weight
                text: model.term
                definition: LinkTools.decorate(model.definition)
                z: model.isSelected ? 5 : 2
            }
        }
    }
}
