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

import Network
import Atoms as A
import Molecules as M
import StyleInfo

M.Page {
    title: "Настройки сети"

    contentItem: Column {

        spacing: Sizes.baseR75

        RowLayout {

            spacing: Sizes.baseR50

            GroupBox {

                label: Control {
                    padding: Sizes.baseR75
                    bottomPadding: 0
                    contentItem: A.AccentText { text: "Настройки сервера" }
                }

                background: Rectangle { color: "transparent"; border.color: Colors.accent; }
                padding: Sizes.baseR75

                contentItem: Column {
                    RowLayout {

                        Switch {
                            scale: Qt.platform.os === "android" ? 2.0 : 1.0
                            checked: false
                            onCheckedChanged: { NetworkManager.synchronization = checked; }
                        }

                        Text {
                            font: Fonts.text
                            color: Colors.white
                            text: NetworkManager.synchronizationState
                        }
                    }

                    M.LabelPair {
                        name: "Ip этого устройства"
                        text: NetworkManager.getFirstLocalIpString()
                    }
                }

            }

        }

        M.TextField {
            labelText: "Ip получателя"
            onTextChanged: { NetworkManager.setReceiverHostIp(text) }
            Component.onCompleted: { text = NetworkManager.getReceiverIp() }
            width: parent.width
        }

        M.LabelPair {
            name: "Состояние подключения"
            text: NetworkManager.connectionState
            width: parent.width
        }

        A.SquareButton {
            text: NetworkManager.isConnected ? "Отключиться" : "Подключиться"
            onClicked:  {
                if(NetworkManager.isConnected) {
                    NetworkManager.disconnectFromHost()
                } else {
                    NetworkManager.connectToHost()
                }
            }
        }
    }
}
