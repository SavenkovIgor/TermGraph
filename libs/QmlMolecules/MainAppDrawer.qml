// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

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

            implicitHeight: contentHeight

            boundsBehavior: Flickable.StopAtBounds

            delegate: ItemDelegate {
                id: groupLstDlgt

                width: ListView.view.width

                contentItem: Text {

                    padding: font.pixelSize / 4

                    color: Colors.white
                    font: Fonts.setWeight(Fonts.h4, Font.Thin)
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
