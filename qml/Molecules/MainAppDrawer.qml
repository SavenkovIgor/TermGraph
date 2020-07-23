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

Drawer {
    id : root

    property alias groupAction: groupMenuButton.action
    signal openSettings

    onOpened: groupMenuButton.forceActiveFocus()

    dragMargin: Qt.styleHints.startDragDistance * 2

    contentItem: ColumnLayout {

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

        Item { Layout.fillHeight: true; }
    }

    background: Rectangle {
        color: Colors.baseLight
        A.RightThinLine { color: Colors.white; }
    }
}
