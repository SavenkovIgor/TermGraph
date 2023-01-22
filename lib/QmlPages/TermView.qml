// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Api

import Atoms as A
import Molecules as M
import Theme

M.Page {
    id: root

    title: "Термин"

    signal editNode()

    header: M.DefaultHeader {

        RoundButton {
            flat: true
            action: Action {
                text: "W"
                onTriggered: {
                    let request = "https://ru.wikipedia.org/w/index.php?search=";
                    request += Api.scene.currentNode.term
                    Qt.openUrlExternally(request);
                }
            }
        }

        A.ToolButton {
            id: editNodeBtn
            visible: !Api.readonlyMode
            action: Action {
                enabled: !Api.readonlyMode
                text: "Изменить вершину"
                shortcut: "Ctrl+E"
                icon.source: Theme.icon.pencil
                onTriggered: root.editNode()
            }
        }
    }

    contentItem:  A.FlickableColumn {
        spacing: 14

        M.LabelPair {
            name: "Название:"
            text: Api.scene.currentNode.term
            visible: text !== ""
        }

        M.LabelPair {
            name: "Определение:"
            text: LinkTools.decorate(Api.scene.currNodeNameAndDefinition)
            visible: text !== ""
        }

        M.LabelPair {
            name: "Иерархия определений:"
            text: Api.scene.currNodeHierarchyDefinition
            visible: text !== ""
        }

        M.LabelPair {
            name: "Описание:"
            text: Api.scene.currentNode.description
            visible: text !== ""
        }

        M.LabelPair {
            name: "Пример:"
            text: Api.scene.currentNode.examples
            visible: text !== ""
        }
    }
}
