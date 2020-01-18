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

import "../Js/Colors.js" as Colors
import "../Js/Fonts.js" as Fonts

Popup {
    id: root

    property alias model: searchList.model

    signal clicked(string nodeUuid)
    signal oneVariant(string nodeUuid)
    signal noResults()

    padding: 0
    height: topPadding + contentItem.height + bottomPadding

    background: Rectangle { color: Colors.baseLight2 }

    function getFirst() { return searchList.getFirst(); }

    contentItem: Item {
        height: searchList.visible ? searchList.contentHeight : nothingFound.height

        Text {
            id: nothingFound
            text: "Ничего не найдено"
            font: Fonts.inputText
            padding: font.pixelSize
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            color: Colors.whiteDisabled
        }

        ListView {
            id: searchList
            anchors.fill: parent
            visible: count !== 0
            focus: true
            onCountChanged: {
                // Notify if only one variant
                if (count === 1)
                    root.oneVariant(getFirst());
                if (count === 0)
                    root.noResults();
            }

            function getFirst() { return count >= 1 ? model[0] : ""; }

            delegate: ItemDelegate {
                background: Rectangle { color: Colors.baseLight2 }
                width: ListView.view.width
                contentItem: Text {
                    topPadding: font.pixelSize
                    bottomPadding: font.pixelSize
                    text: scene.getTermName(modelData)
                    font: Fonts.inputText
                    color: Colors.white
                }

                onClicked: root.clicked(modelData)
            }
        }
    }
}
