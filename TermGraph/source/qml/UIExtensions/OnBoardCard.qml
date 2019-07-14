import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

Pane {
    background: Rectangle { color: appColors.base }

    property real baseSize: 50
    property alias text: tipTitle.text

    Rectangle {
        id: tipFrame

        property real marginVal: baseSize * 0.03

        anchors {
            fill: parent
            margins: marginVal
        }

        radius: baseSize * 0.08
        color: appColors.baseLight3
//        Rectangle { anchors.fill: item; color: "darkcyan"; }

        Item {
            id: item
            anchors {
                fill: parent
                leftMargin: parent.width * 0.1
                rightMargin: parent.width * 0.1
                topMargin: parent.height * 0.1
                bottomMargin: parent.height * 0.1
            }

            ScrollView {
                anchors.fill: parent

                clip: true
                ScrollBar.vertical.policy: ScrollBar.AlwaysOn

                contentItem: TextEdit {
                    id: tipTitle

                    width: parent.width

                    textFormat: TextEdit.RichText
                    wrapMode: TextEdit.WordWrap
                    readOnly: true
                    //                horizontalCenter: parent.horizontalCenter
                    //            anchors.topMargin:

                    font.pixelSize: mainObj.getUiElementSize("inputText")*Screen.pixelDensity
                    color: "white"
                }
            }
        }
    }
}
