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
                id: scrollView
                anchors.fill: parent

                property bool visibleScrollBar: ScrollBar.vertical.active

                clip: true
                ScrollBar.vertical.policy: ScrollBar.AsNeeded

                contentWidth: tipTitle.width
                contentHeight: tipTitle.height

                TextEdit {
                    id: tipTitle

                    width: item.width

                    textFormat: TextEdit.RichText
                    wrapMode: TextEdit.WordWrap
                    readOnly: true

                    font.pixelSize: mainObj.getUiElementSize("inputText") * Screen.pixelDensity
                    color: "white"
                }
            }

            Rectangle {
                anchors.left: scrollView.left
                anchors.bottom: scrollView.bottom

                width: scrollView.width
                height: scrollView.height * 0.05

                gradient: Gradient {
                    GradientStop { position: 0.0; color: "transparent"; }
                    GradientStop { position: 1.0; color: appColors.baseLight3; }
                }
            }
        }
    }
}
