import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Window 2.13
import QtGraphicalEffects 1.0

RoundButton {
    id: root

    property alias color: root.icon.color
    property bool backgroundHidden: false

    property real size: mainObj.getUiElementSize("roundButton") * Screen.pixelDensity

    background: Rectangle {
        radius: width / 2
        color: !root.backgroundHidden ? appColors.white : "transparent"
    }

    width:  implicitWidth
    height: implicitHeight

    icon.width: implicitWidth
    icon.height: implicitHeight

    implicitWidth:  root.size
    implicitHeight: root.size

    padding: 0.28 * width
}

