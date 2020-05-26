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

import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import Helpers 1.0

import "../Atoms" as A
import "../Molecules" as M
import "../Js/Colors.js" as Colors
import "../Js/IconPath.js" as IconPath

M.Page {
    id: root

    property TagProcessor tagTools: TagProcessor { }

    title: "Термин"

    signal editNode()

    header: M.DefaultHeader {

        RoundButton {
            flat: true
            action: Action {
                text: "W"
                onTriggered: {
                    let request = "https://ru.wikipedia.org/w/index.php?search=";
                    request += scene.currentNode.term
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

    contentItem:  A.VerticalFlickable {
        id: flick

        contentWidth:  contentColumn.width
        contentHeight: contentColumn.height

        ColumnLayout {
            id: contentColumn

            spacing: 14
            width: flick.width

            M.LabelPair {
                name: "Dbg:"
                text: scene.getCurrNodeDebugInfo()
                visible: text !== ""
            }

            M.LabelPair {
                name: "Название:"
                text: scene.currentNode.term
                visible: text !== ""
            }

            M.LabelPair {
                name: "Определение:"
                text: tagTools.decorateTags(scene.getCurrNodeNameAndDefinition())
                visible: text !== ""
            }

            M.LabelPair {
                name: "Иерархия определений:"
                text: scene.getCurrNodeHierarchyDefinition()
                visible: text !== ""
            }

            M.LabelPair {
                name: "Описание:"
                text: scene.currentNode.description
                visible: text !== ""
            }

            M.LabelPair {
                name: "Пример:"
                text: scene.currentNode.examples
                visible: text !== ""
            }
        }
    }
}
