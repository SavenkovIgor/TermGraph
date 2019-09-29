import QtQuick 2.13
import QtQuick.Controls 2.13

import "UIExtensions"

MPage {
    id: root

    Component.onCompleted: { databaseVersion.updateText(); }

    header: MainHeader {

        id: mainHeader
        title: "Настройки"

        Component.onCompleted: mainHeader.showArrowIcon()

        onMenuClick: root.StackView.view.pop()
    }

    background: Rectangle { color: appColors.base; }

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
