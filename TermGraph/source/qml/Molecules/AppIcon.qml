import QtQuick 2.13
import QtQuick.Window 2.13
import QtGraphicalEffects 1.13

import "../Js/Colors.js" as Colors
import "../Atoms" as A

Item {
    id: root

    width: 200
    height: 200

    property real aScale: 1

    A.Hex {
        anchors.fill: parent
        scale: 1
        color: Colors.baseLight2

        NumberAnimation on rotation {
            from: 0
            to: 360
            duration: 3000 * root.aScale
            running: false
            loops: Animation.Infinite
        }
    }

    A.Hex {
        anchors.fill: parent
        scale: 0.866
        color: Colors.accent
        rotation: 30

        NumberAnimation on rotation {
            from: 0
            to: 360
            duration: 5000 * root.aScale
            running: false
            loops: Animation.Infinite
        }

    }

    A.Hex {
        anchors.fill: parent
        scale: 0.866 * 0.866
        color: Colors.baseLight
        rotation: 60

        NumberAnimation on rotation {
            from: 0
            to: 360
            running: false
            duration: 4000 * root.aScale
            loops: Animation.Infinite
        }
    }

    DropShadow {
        anchors.fill: text
        source: text
        verticalOffset: 2
        horizontalOffset: 2
        radius: 8.0
        samples: 20
    }

    Text {
        id: text
        anchors.centerIn: parent
        text: "Tg"
        font.pixelSize: 35
        color: "#dcdcdc"
    }
}
