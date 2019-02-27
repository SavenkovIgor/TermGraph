import QtQuick 2.0
import QtQuick.Controls 2.2

import "UIExtensions"

Page {
    id: settingsPage

    property StackView mainStack
    function open() { mainStack.push(settingsPage) }

    header: MainHeader {

        id: mainHeader
        titleText: "Настройки"

        Component.onCompleted: mainHeader.showArrowIcon()

        onMenuClick: mainStack.pop()
    }

    Rectangle {
        anchors.fill: parent
        color: "#332f30"
    }

    Column {

        anchors.fill: parent

        MyLabelPair{
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
        }

        Connections {
            target: networkManager
            onNewOutputConnectionState: {
                connectionState.text = state
                if(networkManager.isConnected()) {
                    connectButton.text = "Отключиться"
                } else {
                    connectButton.text = "Подключиться"
                }
            }
        }

        MySquareButton {
            id: connectButton
            text: "Подключиться"
            onClicked:  {
                if(networkManager.isConnected()) {
                    networkManager.disconnectFromHost()
                } else {
                    networkManager.connectToHost()
                }
            }
        }

        MySquareButton {
            id: initConfigTable
            text: "Создать таблицу конфигурации и записать версию"
            onClicked: mainObj.initConfigTable()
        }

        MySquareButton {
            id: createDatabaseVersionRecord
            text: "Создать запись с версией базы данных в таблице настроек"
            onClicked: mainObj.addDbVersionNumberRecord()
        }
    }
}
