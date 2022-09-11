// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Api

import Atoms as A
import Molecules as M
import Theme

M.Page {
    title: "Настройки сети"

    contentItem: Column {

        spacing: Sizes.base

        RowLayout {

            spacing: Sizes.baseX2

            GroupBox {

                label: Control {
                    padding: Sizes.base
                    bottomPadding: 0
                    contentItem: A.AccentText { text: "Настройки сервера" }
                }

                background: Rectangle { color: "transparent"; border.color: Theme.color.accent; }
                padding: Sizes.base

                contentItem: Column {
                    RowLayout {

                        Switch {
                            scale: Qt.platform.os === "android" ? 2.0 : 1.0
                            checked: false
                            onCheckedChanged: { NetworkManager.synchronization = checked; }
                        }

                        Text {
                            font: Fonts.h4
                            color: Theme.color.white
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
