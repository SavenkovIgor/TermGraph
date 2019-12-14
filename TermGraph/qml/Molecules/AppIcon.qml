import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14
import QtGraphicalEffects 1.14

import "../Js/Colors.js" as Colors
import "../Atoms" as A

Item {
    id: padding

    property int size: 200

    width: size
    height: size

    Item {
        id: root

        width: padding.size * 0.95
        height: padding.size * 0.95
        anchors.centerIn: parent

        property color col1: "#6d9a28"
        property color col2: Colors.accent
        property color col3: Colors.baseLight
        property color colText: "#dcdcdc"

        A.Hex {
            id: hex1
            anchors.centerIn: parent
            width: root.width
            height: root.height
            color: root.col1
        }

        DropShadow {
            anchors.fill: hex1
            source: hex1
            verticalOffset: 4
            horizontalOffset: 4
            radius: 12.0
            samples: 20
        }

        A.Hex {
            id: hex2
            anchors.centerIn: parent
            width: parent.width * 0.866
            height: parent.height * 0.866
            color: root.col2
            rotation: 30
        }

        DropShadow {
            anchors.fill: hex2
            source: hex2
            verticalOffset: 3
            horizontalOffset: 3
            radius: 10.0
            samples: 20
            rotation: 30
        }

        A.Hex {
            id: hex3
            anchors.centerIn: parent
            width: parent.width * Math.pow(0.866, 2)
            height: parent.height * Math.pow(0.866, 2)
            color: root.col3
            rotation: 60
        }

        DropShadow {
            anchors.fill: hex3
            source: hex3
            verticalOffset: 2
            horizontalOffset: 2
            radius: 8.0
            samples: 20
            rotation: 60
        }

        Text {
            id: text
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -root.height * 0.02
            text: "Tg"
            font.pixelSize: root.height * 0.35
            color: root.colText
        }

        DropShadow {
            anchors.fill: text
            source: text
            verticalOffset: 2
            horizontalOffset: 2
            radius: 8.0
            samples: 20
        }
    }
}
