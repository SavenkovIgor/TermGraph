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

import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

import "UIExtensions"
import "Js/Colors.js" as Colors

MPage {
    id: root

    header: MainHeader {
        id: mainHeader
        title: "Термин"

        Component.onCompleted: mainHeader.showArrowIcon()

        onMenuClick: root.StackView.view.pop()
    }

    contentItem: ScrollView {
        id: scroll

        clip: true
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        ColumnLayout {
            spacing: 14
            width: scroll.width

            MyLabelPair {
                name: "Dbg:"
                text: sceneObj.getCurrNodeDebugInfo()
                visible: text !== ""
            }

            MyLabelPair {
                name: "Название:"
                text: sceneObj.currentNode.term
                visible: text !== ""
            }

            MyLabelPair {
                name: "Определение:"
                text: tagProcessor.decorateTags(sceneObj.getCurrNodeNameAndDefinition())
                visible: text !== ""
            }

            MyLabelPair {
                name: "Иерархия определений:"
                text: sceneObj.getCurrNodeHierarchyDefinition()
                visible: text !== ""
            }

            MyLabelPair {
                name: "Описание:"
                text: sceneObj.currentNode.description
                visible: text !== ""
            }

            MyLabelPair {
                name: "Пример:"
                text: sceneObj.currentNode.examples
                visible: text !== ""
            }
        }
    }

    background: Rectangle { color: Colors.base }
}
