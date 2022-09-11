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
    property real weight: 1

    color: {
        switch (type) {
        case NodeType.Orphan:     return Qt.tint(Theme.color.term.orphanNode, Theme.color.stdTint)
        case NodeType.Root:       return Qt.tint(Theme.color.term.rootNode,   Theme.color.stdTint)
        case NodeType.EndLeaf:    return Qt.tint(Theme.color.term.leafNode,   Theme.color.stdTint)
        case NodeType.MiddleLeaf: return Qt.tint(Theme.color.term.leafNode,   Theme.color.stdTint)
        }
    }

    x: rect.x
    y: rect.y
    width: rect.width
    height: rect.height

    border { width: (1 * weight) + 1; color: Theme.color.black; }

    Text {
        id: termLabel
        anchors { fill: parent }
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: Theme.color.white
        font: Theme.font.term
    }
}
