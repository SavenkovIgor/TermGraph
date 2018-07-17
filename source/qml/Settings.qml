import QtQuick 2.0
import QtQuick.Controls 2.2

import "UIExtensions"

Item {

    Text {
        id: txtLabel
        anchors {
            left:  parent.left
            right: parent.right
            top:   parent.top
        }

        text: qsTr("Установить расположение файла базы данных")
    }

    Text {
        anchors {
            left: parent.left
            top: txtLabel.bottom
        }

        id: currLocD
        text: "Текущее расположение:"
    }

    Text {
        text: "//someDir"

        anchors {
            left: currLocD.right
            right: parent.right
            top: txtLabel.bottom

        }
    }

    Button{
        id: srchBtn
        anchors {
            left:   parent.left
            right:  parent.right
            top:    currLocD.bottom
        }

        text: "Обзор..."

        onClicked:  { fileDialog.open() }
    }

    MyLabelPair{
        id: localIpDescription
        name: "Ip этого устройства:"
        text: networkManager.getFirstLocalIpString()
        anchors {
            left: parent.left
            right: parent.right
            top: srchBtn.bottom
        }
    }

    MyTextField {
        id: receiverIp

        anchors {
            left: parent.left
            right: parent.right
            top: localIpDescription.bottom
            bottom: parent.bottom
        }

        labelText: "Ip получателя"
        text: "192.168.0.80"

        onTextChanged: { networkManager.setReceiverHostIp(text) }
    }

    /*
    Button{
        anchors {
            left: parent.left
            right: parent.right
            top: srchBtn.bottom
            bottom: parent.bottom
        }

        text: "Тест"
        onClicked: sceneObj.createTestGroups()

    }
    */
}
