// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Api

import Theme
import Atoms as A

A.DrawerPage {
    id: root

    signal searchResultClicked(string nodeUuid)

    leftPadding:  Sizes.baseX2

    onAboutToShow: {
        searchText.clear();
        searchText.forceActiveFocus();
    }

    contentItem: ColumnLayout {

        RowLayout {
            TextField {
                id : searchText
                Layout.fillWidth: true
                font: Theme.font.h3
                selectByMouse: true

                color: Theme.color.white
                placeholderTextColor: Theme.color.whiteDisabled
                placeholderText: "Поиск"

                Keys.onReturnPressed: searchList.forceActiveFocus();
            }

            A.ToolButton { icon.source: Theme.icon.magnifyingGlass }
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

                model: Scene.search(searchReq)
                boundsBehavior: Flickable.StopAtBounds

                delegate: ItemDelegate {
                    id: item
                    width: ListView.view.width

                    highlighted: ListView.isCurrentItem

                    contentItem: Text {
                        topPadding: font.pixelSize / 3
                        bottomPadding: font.pixelSize / 3
                        text: Scene.termUuidToName(modelData)
                        font: Theme.font.h3
                        color: Theme.color.white
                    }

                    background: Rectangle {
                        color: Theme.color.baseLight3
                        visible: item.highlighted
                    }

                    Keys.onReturnPressed: root.searchResultClicked(modelData)
                    onClicked: root.searchResultClicked(modelData)
                }
            }

            Text {
                text: "Ничего не найдено"
                font: Theme.font.h3
                padding: font.pixelSize
                anchors.fill: searchList
                color: Theme.color.whiteDisabled
                visible: searchList.count === 0

                Layout.fillHeight: true
            }
        }
    }
}
