// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import Atoms as A
import Molecules as M
import StyleInfo

M.Page {
    title: "Режим изучения"

    background: Rectangle { color: Colors.base }

    A.RoundButton {
        id: addGroupBtn
        anchors { right: parent.right; bottom: parent.bottom; margins: width / 2; }
        icon.source: IconPath.rightArrow
    }

    contentItem: ListView {
        id: groupsList
        model: ["Названия терминов"]
        keyNavigationEnabled: true

        highlight: Rectangle {
            width: 200; height: 20
            color: Colors.baseLight2
            y: groupsList.currentItem.y;
        }

        delegate: Control {

            property string groupUuid: modelData

            anchors.fill: parent

            background: Rectangle {
                color: "transparent"
                A.BottomThinLine { }
            }

            contentItem: Text {
                anchors.fill: parent
                leftPadding: font.pixelSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft

                color: Colors.white
                font: Fonts.setWeight(Fonts.h3, Font.Thin)

                text: modelData

                MouseArea{ anchors.fill: parent }  // TODO: Make action here
            }
        }
    }
}
