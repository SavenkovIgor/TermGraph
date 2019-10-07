import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Templates 2.13 as T
import QtQuick.Window 2.13

import "../Js/Colors.js" as Colors

T.Control {
    id: root

    property real baseSize: 50
    property real minSizing: Math.min(width, height);

    background: Rectangle { color: Colors.base; }

    contentItem: Rectangle {
        property real marginVal: root.minSizing * 0.05

        anchors { fill: parent; margins: marginVal; }

        radius: root.minSizing * 0.08
        color: Colors.baseLight3

        Column {
            id: item
            anchors {
                fill: parent
                leftMargin: parent.width * 0.1
                rightMargin: parent.width * 0.1
                topMargin: parent.height * 0.1
                bottomMargin: parent.height * 0.1
            }

            Text {
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                textFormat: TextEdit.RichText
                text: "Карточки"

                font.pixelSize: mainObj.getUiElementSize("appHeader") * Screen.pixelDensity
                color: "white"
            }

            Text {
                topPadding: 40
                font.pixelSize: mainObj.getUiElementSize("inputText") * Screen.pixelDensity;
                color: "white"
                width: parent.width
                wrapMode: Text.Wrap
                text: "В упражнении нужно постараться вспомнить определение " +
                      "на карточке, а затем оценить, насколько вы были уверены в ответе. " +
                      "В зависимости от оценки, карточки в дальнейшем будут показываться чаще или реже."
            }

            Button {
                text: "Погнали"
                font.pixelSize: mainObj.getUiElementSize("inputText") * Screen.pixelDensity;
            }
        }
    }
}
