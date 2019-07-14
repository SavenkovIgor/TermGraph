import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0

RoundButton {
    id: root

    property alias color: root.icon.color
    property bool backgroundHidden: false

    property string iconName: ""

    onIconNameChanged: loadIcon()

    function loadIcon() {
        // Take object width
        var sz = width - (2 * padding)
        icon.width  = Math.floor(sz)
        icon.height = Math.floor(sz)

        var prefix = getPrefixForSize(sz)
        icon.source = "qrc:/icons/" + iconName + prefix + ".png"
    }

    function getPrefixForSize(size) {
        // file suffixes
        // 8() 16(-2x) 24(-3x) 32(-4x) 48(-6x) 64(-8x)

        // Middles:
        // 8  - 16: 12
        // 16 - 24: 20
        // 24 - 32: 28
        // 32 - 48: 40
        // 48 - 64: 56

        if (size <= 12)
            return ""
        else if (size <= 20)
            return "-2x"
        else if (size <= 28)
            return "-3x"
        else if (size <= 40)
            return "-4x"
        else if (size <= 56)
            return "-6x"
        else
            return "-8x"
    }

    anchors.margins: width/2
    z: 1

    background: Rectangle {
        id: back
        radius: width / 2
        color: !root.backgroundHidden ? appColors.white : "transparent"
    }

    width:  mainObj.getUiElementSize("roundButton")*Screen.pixelDensity
    height: mainObj.getUiElementSize("roundButton")*Screen.pixelDensity

    implicitWidth: width
    implicitHeight: height

    padding: 0.27*width
}

