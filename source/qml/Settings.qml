import QtQuick 2.0
import QtQuick.Controls 2.2

import "UIExtensions"

Item {

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
    }
}
