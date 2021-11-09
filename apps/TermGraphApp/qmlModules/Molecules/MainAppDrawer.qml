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

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Api

import StyleInfo
import Atoms as A
import Tools

Drawer {
    id : root

    required property int maxWidth
    property alias groupAction: groupMenuButton.action

    signal openSettings

    onOpened: groupMenuButton.forceActiveFocus()

    dragMargin: Qt.styleHints.startDragDistance * 2
    width: Tools.clamp(implicitWidth * 1.3, implicitWidth, root.maxWidth)

    contentItem: A.FlickableColumn {
        spacing: 0

        SideMenuHeader {
            Layout.fillWidth: true
            onSettingsClicked: root.openSettings()
        }

        A.SideMenuButton {
            id: groupMenuButton
            text: action.text
            icon.source: action.icon.source
            Layout.fillWidth: true
        }

        A.SideMenuButton {
            text: "Изучение"
            icon.source: IconPath.book
            Layout.fillWidth: true
            visible: false
        }

        A.SideMenuButton {
            text: "Синхронизация"
            icon.source: IconPath.loopCircular
            Layout.fillWidth: true
            visible: false
        }

        ListView {
            id: groupListView

            property real maxWidth: 0

            Layout.fillWidth: true

            keyNavigationEnabled: true
            model: GroupsManager.allUuidSorted

            height: contentHeight

            boundsBehavior: Flickable.StopAtBounds

            delegate: ItemDelegate {
                id: groupLstDlgt

                width: ListView.view.width

                contentItem: Text {

                    padding: font.pixelSize / 4

                    color: Colors.white
                    font: Fonts.setWeight(Fonts.text, Font.Thin)
                    fontSizeMode: Text.HorizontalFit
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft

                    text: GroupsManager.getGroupName(modelData)
                }

                background: Rectangle {
                    color: groupLstDlgt.ListView.isCurrentItem ? "darkGray" : "transparent"
                    A.BottomThinLine { color: Colors.white }
                }

                Keys.onReturnPressed: apply()
                onClicked: apply()

                function apply() {
                    root.close();
                    Scene.selectGroup(modelData);
                }
            }
        }
    }

    background: Rectangle {
        color: Colors.baseLight
        A.RightThinLine { color: Colors.white; }
    }
}
