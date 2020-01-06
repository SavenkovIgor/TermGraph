import QtQuick 2.13

import "../Js/Fonts.js" as Fonts
import "../Js/Colors.js" as Colors

Rectangle {

    property rect rect: Qt.rect(10, 10, 50, 30)
    property alias text: termLabel.text

    x: rect.x
    y: rect.y
    width: rect.width
    height: rect.height

    z: 2

    border { width: 1; color: Colors.black; }

    Text {
        id: termLabel
        anchors { fill: parent }
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: Colors.white
        font: Fonts.term
    }
}
