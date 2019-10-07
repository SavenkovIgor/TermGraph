import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Templates 2.13 as T
import QtQuick.Window 2.13

import "../UIExtensions"
import "../Js/Colors.js" as Colors

MPage {
    id: root

    property alias text: tipTitle.text
    property real minSizing: Math.min(width, height);

    background: Rectangle { color: Colors.base }

    contentItem: ColumnLayout {
        property real marginVal: root.minSizing * 0.05

        anchors { fill: parent; margins: marginVal; }

        radius: root.minSizing * 0.08
        color: Colors.baseLight3

        Item {
            id: card
            anchors {
                fill: parent
                leftMargin: parent.width * 0.1
                rightMargin: parent.width * 0.1
                topMargin: parent.height * 0.1
                bottomMargin: parent.height * 0.1
            }

            Text {
            }

            Rectangle {
                id: line
            }

            Text {
                color: "transparent"
            }
        }

        RowLayout {
            Button {
                id: badLearn
                Layout.fillWidth: true
            }

            Button {
                id: wellLearn
                Layout.fillWidth: true
            }

            Button {
                id: excellentLearn
                Layout.fillWidth: true
            }
        }

    }
}
