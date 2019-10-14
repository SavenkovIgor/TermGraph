import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Templates 2.13 as T
import QtQuick.Window 2.13

import "../UIExtensions"
import "../Js/Colors.js" as Colors

MPage {
    id: root

    background: Rectangle { color: Colors.base }

    contentItem: ColumnLayout {

        spacing: root.height * 0.05

        Rectangle {
            id: card

            radius: root.width * 0.02
            color: Colors.baseLight3

            Layout.fillHeight: true
            Layout.fillWidth: true

            anchors.margins: root.height * 0.05

            ColumnLayout {
                anchors.fill: parent

                Text {
                    text: card
                    Layout.fillHeight: true
                }

                Rectangle {
                    id: line
                    height: 5
                    color: "red"
                }

                Text {
                    height: card.height * 0.2
                    color: "transparent"
                }
            }
        }

        RowLayout {

            height: root.height * 0.2
            Layout.fillWidth: true

            spacing: root.height * 0.05

            Button {
                id: badLearn
                text: "Button1"

                background: Rectangle { color: Colors.baseLight3 }
                Layout.fillWidth: true
            }

            Button {
                id: wellLearn
                text: "Button2"

                background: Rectangle { color: Colors.baseLight3 }
                Layout.fillWidth: true
            }

            Button {
                id: excellentLearn
                text: "Button3"

                background: Rectangle { color: Colors.baseLight3 }
                Layout.fillWidth: true
            }
        }

    }
}
