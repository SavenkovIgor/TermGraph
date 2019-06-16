import QtQuick 2.0
import QtQuick.Controls 2.5

Pane {
    background: Rectangle {
        color: appColors.base
    }

    property real baseSize: 50

    Rectangle {
        id: tipFrame

        property real marginVal: baseSize * 0.03

        anchors.fill: parent

        anchors.topMargin: marginVal
        anchors.bottomMargin: marginVal
        anchors.leftMargin: marginVal
        anchors.rightMargin: marginVal

        radius: baseSize * 0.08
        color: appColors.baseLight3

        Text {
            id: tip1title
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: tip1title.font.pixelSize * 3
            text: "Шаг 1. Добавьте группу!"
            color: "white"
        }
    }
}