import QtQuick 2.13
import QtQuick.Window 2.13

Window {
    id: root

    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    property real aScale: 2.5

    Hex {
        anchors.centerIn: parent
        scale: 1
        color: "red"

        NumberAnimation on rotation {
            from: 0
            to: 360
            duration: 3000 * root.aScale
            loops: Animation.Infinite
        }

        Hex {
            anchors.centerIn: parent
            scale: 0.866
            color: "blue"
            rotation: 30

            NumberAnimation on rotation {
                from: 0
                to: 360
                duration: 5000 * root.aScale
                loops: Animation.Infinite
            }

            Hex {
                anchors.centerIn: parent
                scale: 0.866
                color: "green"
                rotation: 30

                NumberAnimation on rotation {
                    from: 0
                    to: 360
                    duration: 4000 * root.aScale
                    loops: Animation.Infinite
                }
            }
        }
    }
}
