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

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import StyleInfo 1.0
import "../Atoms" as A

A.DrawerPage {
    id: root

    signal searchResultClicked(string nodeUuid)

    leftPadding:  Sizes.baseR50

    onAboutToShow: {
        searchText.clear();
        searchText.forceActiveFocus();
    }

    contentItem: ColumnLayout {

        RowLayout {
            TextField {
                id : searchText
                Layout.fillWidth: true
                font: Fonts.inputText
                selectByMouse: true

                color: Colors.white
                placeholderTextColor: Colors.whiteDisabled
                placeholderText: "Поиск"

                Keys.onReturnPressed: searchList.forceActiveFocus();
            }

            A.ToolButton { icon.source: IconPath.magnifyingGlass }
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            ListView {
                id: searchList
                anchors.fill: parent
                visible: count !== 0
                focus: true

                property string searchReq: searchText.displayText

                model: scene.search(searchReq)
                boundsBehavior: Flickable.StopAtBounds

                delegate: ItemDelegate {
                    id: item
                    width: ListView.view.width

                    highlighted: ListView.isCurrentItem

                    contentItem: Text {
                        topPadding: font.pixelSize / 3
                        bottomPadding: font.pixelSize / 3
                        text: scene.termUuidToName(modelData)
                        font: Fonts.inputText
                        color: Colors.white
                    }

                    background: Rectangle {
                        color: Colors.baseLight3
                        visible: item.highlighted
                    }

                    Keys.onReturnPressed: root.searchResultClicked(modelData)
                    onClicked: root.searchResultClicked(modelData)
                }
            }

            Text {
                text: "Ничего не найдено"
                font: Fonts.inputText
                padding: font.pixelSize
                anchors.fill: searchList
                color: Colors.whiteDisabled
                visible: searchList.count === 0

                Layout.fillHeight: true
            }
        }
    }
}
