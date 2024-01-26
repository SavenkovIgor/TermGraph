// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls as C
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
            C.TextField {
                id : searchText
                Layout.fillWidth: true
                font: Theme.font.h3
                selectByMouse: true

                color: Theme.color.base7
                placeholderTextColor: Theme.color.base4
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

                model: Api.scene.search(searchReq)
                boundsBehavior: Flickable.StopAtBounds

                delegate: C.ItemDelegate {
                    id: item
                    width: ListView.view.width

                    highlighted: ListView.isCurrentItem

                    contentItem: Text {
                        topPadding: font.pixelSize / 3
                        bottomPadding: font.pixelSize / 3
                        text: Api.scene.termUuidToName(modelData)
                        font: Theme.font.h3
                        color: Theme.color.base7
                    }

                    background: Rectangle {
                        color: Theme.color.base3
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
                color: Theme.color.base4
                visible: searchList.count === 0

                Layout.fillHeight: true
            }
        }
    }
}
