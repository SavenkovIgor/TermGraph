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

import "UIExtensions"
import "Js/Colors.js" as Colors

MPage {
    id: root

    Component.onCompleted: { databaseVersion.updateText(); }

    header: MainHeader {

        id: mainHeader
        title: "Настройки"

        Component.onCompleted: mainHeader.showArrowIcon()

        onMenuClick: root.StackView.view.pop()
    }

    contentItem: Column {
        property real expectedWidth: width - leftPadding - rightPadding

        MyLabelPair {
            id: localIpDescription
            name: "Ip этого устройства:"
            text: networkManager.getFirstLocalIpString()
            width: parent.expectedWidth
        }

        MyTextField {
            id: receiverIp
            labelText: "Ip получателя"
            onTextChanged: { networkManager.setReceiverHostIp(text) }
            Component.onCompleted: { text = networkManager.getReceiverIp() }
            width: parent.expectedWidth
        }

        MyLabelPair{
            id: connectionState
            name: "Состояние подключения:"
            text: networkManager.connectionState
            width: parent.expectedWidth
        }

        MySquareButton {
            id: connectButton
            text: networkManager.isConnected ? "Отключиться" : "Подключиться"
            onClicked:  {
                if(networkManager.isConnected) {
                    networkManager.disconnectFromHost()
                } else {
                    networkManager.connectToHost()
                }
            }
        }

        MyLabelPair {
            id: databaseVersion
            name: "Версия базы данных:"
            text: mainObj.dbVersion()
            width: parent.expectedWidth

            function updateText() {
                text = mainObj.dbVersion()
            }
        }
    }
}
