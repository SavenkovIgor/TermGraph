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
                    request += Scene.currentNode.term
                    Qt.openUrlExternally(request);
                }
            }
        }

        A.ToolButton {
            id: editNodeBtn
            action: Action {
                text: "Изменить вершину"
                shortcut: "Ctrl+E"
                icon.source: IconPath.pencil
                onTriggered: root.editNode()
            }
        }
    }

    contentItem:  A.FlickableColumn {
        spacing: 14

        M.LabelPair {
            name: "Название:"
            text: Scene.currentNode.term
            visible: text !== ""
        }

        M.LabelPair {
            name: "Определение:"
            text: LinkTools.decorate(Scene.currNodeNameAndDefinition)
            visible: text !== ""
        }

        M.LabelPair {
            name: "Иерархия определений:"
            text: Scene.currNodeHierarchyDefinition
            visible: text !== ""
        }

        M.LabelPair {
            name: "Описание:"
            text: Scene.currentNode.description
            visible: text !== ""
        }

        M.LabelPair {
            name: "Пример:"
            text: Scene.currentNode.examples
            visible: text !== ""
        }
    }
}
