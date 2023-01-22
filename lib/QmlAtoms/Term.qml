// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick

import Theme

import Api

Rectangle {
    id: root

    property bool isSelected: false
    property int type: NodeType.MiddleLeaf
    property rect rect: Qt.rect(10, 10, 50, 30)
    property alias text: termLabel.text
    property alias definition: definitionLabel.text
    property real weight: 1
    property bool hasDefinition: definition !== ""

    readonly property color textColor: Theme.color.base7

    color: Theme.color.base1

    x: rect.x
    y: rect.y
    width: rect.width
    height: rect.height

    Rectangle {
        radius: parent.radius
        anchors.fill: termLabel
        color: {
            switch (type) {
            case NodeType.Orphan:     return Theme.color.term.orphanNode;
            case NodeType.Root:       return Theme.color.term.rootNode;
            case NodeType.EndLeaf:    return Theme.color.term.leafNode;
            case NodeType.MiddleLeaf: return Theme.color.term.leafNode;
            }
        }
    }

    Text {
        id: termLabel
        anchors { left: parent.left; right: parent.right; top: parent.top; bottom: root.hasDefinition ? undefined : parent.bottom }
        leftPadding: 16
        rightPadding: 16
        topPadding: 8
        bottomPadding: 8
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: root.textColor
        font: Theme.font.term
        wrapMode: Text.Wrap
    }

    Text {
        id: definitionLabel
        anchors { top: termLabel.bottom; topMargin: 8; left: parent.left; right: parent.right; bottom: parent.bottom; }
        leftPadding: 16
        rightPadding: 16
        bottomPadding: 8
        visible: root.hasDefinition
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        color: root.textColor
        font: Theme.font.term
        wrapMode: Text.Wrap
    }
}
