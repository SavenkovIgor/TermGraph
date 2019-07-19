import QtQuick 2.12
import QtQuick.Controls 2.12

import "UIExtensions"
// primitive -> element -> component -> view
Page {
    id: root

    Component.onCompleted: { databaseVersion.updateText(); }

    header: MainHeader {

        id: mainHeader
        titleText: "Настройки"

        Component.onCompleted: mainHeader.showArrowIcon()

        onMenuClick: root.StackView.view.pop()
    }

    background: Rectangle { color: appColors.base; }

    contentItem: Column {
        MyLabelPair {
            id: localIpDescription
            name: "Ip этого устройства:"
            text: networkManager.getFirstLocalIpString()
        }

        MyTextField {
            id: receiverIp
            labelText: "Ip получателя"
            onTextChanged: { networkManager.setReceiverHostIp(text) }
            Component.onCompleted: { text = networkManager.getReceiverIp() }
        }

        MyLabelPair{
            id: connectionState
            name: "Состояние подключения:"
            text: networkManager.connectionState
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

            function updateText() {
                text = mainObj.dbVersion()
            }
        }
    }
}
