import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Window 2.13
import QtGraphicalEffects 1.0

RoundButton {
    id: root

    property alias color: root.icon.color
    property bool backgroundHidden: false

    anchors.margins: width / 2
    z: 1

    background: Rectangle {
        id: back
        radius: width / 2
        color: !root.backgroundHidden ? appColors.white : "transparent"
    }

    width:  implicitWidth
    height: implicitHeight

    implicitWidth:  mainObj.getUiElementSize("roundButton") * Screen.pixelDensity
    implicitHeight: mainObj.getUiElementSize("roundButton") * Screen.pixelDensity

    padding: 0.27 * width
}

