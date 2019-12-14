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
import QtQuick.Layouts 1.14

import "../Molecules" as M
import "../Js/Colors.js" as Colors

M.Page {
    id: root

    title: "Термин"

    contentItem: ScrollView {
        id: scroll

        contentWidth: root.implicitContentWidth

        clip: true
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        ColumnLayout {
            spacing: 14
            width: scroll.width

            M.LabelPair {
                name: "Dbg:"
                text: sceneObj.getCurrNodeDebugInfo()
                visible: text !== ""
            }

            M.LabelPair {
                name: "Название:"
                text: sceneObj.currentNode.term
                visible: text !== ""
            }

            M.LabelPair {
                name: "Определение:"
                text: tagProcessor.decorateTags(sceneObj.getCurrNodeNameAndDefinition())
                visible: text !== ""
            }

            M.LabelPair {
                name: "Иерархия определений:"
                text: sceneObj.getCurrNodeHierarchyDefinition()
                visible: text !== ""
            }

            M.LabelPair {
                name: "Описание:"
                text: sceneObj.currentNode.description
                visible: text !== ""
            }

            M.LabelPair {
                name: "Пример:"
                text: sceneObj.currentNode.examples
                visible: text !== ""
            }
        }
    }
}
