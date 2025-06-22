// Copyright © 2016-2025. Savenkov Igor
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
                            onCheckedChanged: { Api.net.synchronization = checked; }
                        }

                        Text {
                            font: Theme.font.h4
                            color: Theme.color.base7
                            text: Api.net.synchronizationState
                        }
                    }

                    M.LabelPair {
                        name: "Ip этого устройства"
                        text: Api.net.getFirstLocalIpString()
                    }
                }

            }

        }

        M.TextField {
            labelText: "Ip получателя"
            onTextChanged: { Api.net.setReceiverHostIp(text) }
            Component.onCompleted: { text = Api.net.getReceiverIp() }
            width: parent.width
        }

        M.LabelPair {
            name: "Состояние подключения"
            text: Api.net.connectionState
            width: parent.width
        }

        A.SquareButton {
            text: Api.net.isConnected ? "Отключиться" : "Подключиться"
            onClicked:  {
                if(Api.net.isConnected) {
                    Api.net.disconnectFromHost()
                } else {
                    Api.net.connectToHost()
                }
            }
        }
    }
}
