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
import QtQuick.Window 2.13
import QtQuick.Dialogs 1.3

import "../UIExtensions"
import "../Js/IconPath.js" as IconPath
import "../Js/Colors.js" as Colors

Page {
    id: root

    header: MainHeader {

        id: mainHeader
        title: "Режим изучения"

        Component.onCompleted: mainHeader.showArrowIcon()

        onMenuClick: root.StackView.view.pop()
    }

    background: Rectangle { color: Colors.base }

    MyRoundButton {
        id: addGroupBtn
        anchors { right: parent.right; bottom: parent.bottom; margins: width / 2; }
        icon.source: IconPath.rightArrow
    }

    contentItem: ListView {
        id: groupsList
        model: ["Названия вершин"]
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
                ThinLine { target: parent; side: ThinLine.LineSide.Bottom; }
            }

            contentItem: Text {
                anchors.fill: parent
                leftPadding: font.pixelSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft

                text: modelData
                color: Colors.white
                font {
                    weight: Font.Thin
                    pixelSize: mainObj.getUiElementSize("capitalText") * Screen.pixelDensity
                }

                MouseArea{ anchors.fill: parent }  // TODO: Make action here
            }
        }
    }
}
