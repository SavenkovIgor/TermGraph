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

import "../Atoms"
import "../Molecules"
import "../Js/IconPath.js" as IconPath
import "../Js/Colors.js" as Colors

ToolBar {
    id: mainHeader

    signal menuClick
    signal checkClick

    background: Rectangle {
        anchors.fill: parent
        color: Colors.baseLight
    }

    property alias title: titleLabel.text

    function showMenuIcon()  { mainMenuButton.buttonState = MBurgerButton.IconState.Burger }
    function showArrowIcon() { mainMenuButton.buttonState = MBurgerButton.IconState.Back }

    function showCheckButton() {
        checkButton.visible = true
    }

    RowLayout {
        spacing: 20
        anchors.fill: parent

        MBurgerButton {
            id: mainMenuButton
            onClicked: mainHeader.menuClick()
        }

        Label {
            id: titleLabel

            font.pixelSize: mainObj.getUiElementSize("appHeader")
            minimumPixelSize: font.pixelSize / 2
            fontSizeMode: Text.Fit
            elide: Label.ElideRight
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            Layout.fillWidth: true
        }

        MToolButton {
            id: checkButton
            icon.source: IconPath.check
            visible: false

            onClicked: mainHeader.checkClick()
        }
    }
}
