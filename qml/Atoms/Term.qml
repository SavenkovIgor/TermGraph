// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Effects

import Theme

import Api

Rectangle {
    id: root

    property bool isSelected: false
    property int type: NodeType.MiddleLeaf
    property int area: KnowledgeArea.None
    property rect rect: Qt.rect(10, 10, 50, 30)
    property var termAndSynonyms: []
    property bool hasSynonyms: termAndSynonyms.length > 1
    property alias definition: definitionLabel.text
    property real weight: 1
    property bool hasDefinition: definition !== ""

    readonly property color textColor: Theme.color.base7
    readonly property bool hasArea: area !== KnowledgeArea.None

    color: Theme.color.base1

    x: rect.x
    y: rect.y
    width: rect.width
    height: rect.height

    Rectangle {
        radius: parent.radius
        anchors.fill: root.hasSynonyms ? termColumn : termLabel
        color: {
            switch (type) {
            case NodeType.Orphan:     return Theme.color.term.orphanNode;
            case NodeType.Root:       return Theme.color.term.rootNode;
            case NodeType.EndLeaf:    return Theme.color.term.leafNode;
            case NodeType.MiddleLeaf: return Theme.color.term.leafNode;
            case NodeType.Ghost:      return Theme.color.term.ghostNode;
            }
        }
    }

    component TermText: Text {
        leftPadding: 16
        rightPadding: 16
        color: root.textColor
        font: Theme.font.term
        wrapMode: Text.Wrap
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }

    TermText {
        id: termLabel
        visible: !root.hasSynonyms
        anchors { left: parent.left; right: parent.right; top: parent.top; bottom: root.hasDefinition ? undefined : parent.bottom }
        rightPadding: root.hasArea ? 32 : 16
        topPadding: 8
        bottomPadding: 8
        text: root.termAndSynonyms[0]

        Image {
            id: areaIcon
            visible: root.hasArea
            width: 16
            height: 16
            anchors { right: parent.right; rightMargin: 8; verticalCenter: parent.verticalCenter; }
            source: Theme.areaIcon(root.area)
            layer.enabled: false
            layer.effect: MultiEffect {
                colorizationColor: "white"
                colorization: 1.0
            }
        }
    }

    Column {
        id: termColumn
        visible: root.hasSynonyms

        anchors {
            left: parent.left;
            right: parent.right;
            top: parent.top;
            bottom: root.hasDefinition ? undefined : parent.bottom
        }

        topPadding: 4
        bottomPadding: 4

        Repeater {
            model: root.termAndSynonyms

            TermText {
                id: term
                readonly property bool underline: index < root.termAndSynonyms.length - 1
                width: parent.width
                topPadding: 4
                bottomPadding: 4
                text: modelData

                Rectangle {
                    visible: parent.underline
                    anchors.bottom: parent.bottom
                    height: 1
                    width: parent.width
                }
            }
        }
    }

    TermText {
        id: definitionLabel
        visible: root.hasDefinition
        anchors {
            top: root.hasSynonyms ? termColumn.bottom : termLabel.bottom;
            topMargin: 8;
            left: parent.left;
            right: parent.right;
            bottom: parent.bottom;
        }
        bottomPadding: 8
    }
}
