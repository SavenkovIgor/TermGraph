import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Templates 2.13 as T
import QtQuick.Window 2.13

T.Control {
    id: root

    property real baseSize: 50
    property alias text: tipTitle.text
    property real minSizing: Math.min(width, height);

    background: Rectangle { color: appColors.base; }

    contentItem: Rectangle {
        property real marginVal: root.minSizing * 0.05

        anchors { fill: parent; margins: marginVal; }

        radius: root.minSizing * 0.08
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
