import QtQuick 2.12
import QtQuick.Controls 2.12

import "UIExtensions"

Page {
    id: settingsPage

    property StackView mainStack

    function open() {
        mainStack.push(settingsPage)
        openActions()
    }

    function openActions() {
        databaseVersion.updateText()
    }

    header: MainHeader {

        id: mainHeader
        titleText: "Настройки"

        Component.onCompleted: mainHeader.showArrowIcon()

        onMenuClick: mainStack.pop()
    }

    Rectangle {
        anchors.fill: parent
        color: appColors.base
    }

    Column {

        anchors.fill: parent

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
