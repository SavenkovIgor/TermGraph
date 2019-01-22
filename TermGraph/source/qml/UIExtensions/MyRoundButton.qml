import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Window 2.3
import QtGraphicalEffects 1.0

RoundButton {
    id: root
    function loadIcon( nameUrl ) {
        var sz = width - ( 2*padding ) //need icon width

        var prefix = getPrefixForSize(sz)

        iconImg.width  = Math.floor(sz)
        iconImg.height = Math.floor(sz)

//        console.log(iconImg.width)
//        console.log(iconImg.height)

        iconImg.source = nameUrl + prefix + ".png"
    }

    function getPrefixForSize( size ) {
        //8 16 24 32 48 64 sizes
        //8  ""
        //16 "-2x"
        //24 "-3x"
        //32 "-4x"
        //48 "-6x"
        //64 "-8x"

        //Middles:
        //8  - 16: 12
        //16 - 24: 20
        //24 - 32: 28
        //32 - 48: 40
        //48 - 64: 56

        if( size <= 12 )
            return ""
        else if ( size <= 20 )
            return "-2x"
        else if ( size <= 28 )
            return "-3x"
        else if ( size <= 40 )
            return "-4x"
        else if ( size <= 56 )
            return "-6x"
        else
            return "-8x"
    }

    function hideBack() {
        back.color = "#00000000"
    }

    anchors.margins: width/2
    z: 1

    background: Rectangle {
        id: back
        radius: width/2
        color: {
            if( root.checkable == true ) {
                if( root.checked ) {
                    border.width = 1
                    return "#bababa"
                } else {
                    border.width = 0
                    return "#00000000"
                }

            }

            return "#fffff0"
        }
    }

    width:  mainObj.getUiElementSize("roundButton")*Screen.pixelDensity
    height: mainObj.getUiElementSize("roundButton")*Screen.pixelDensity

    implicitWidth: width
    implicitHeight: height

    padding: 0.27*width

    property alias color: colorRect.color

    Rectangle {
        id: colorRect
        anchors.fill: iconImg
        color: "#000000"
        visible: false
    }

    Image {
        id: iconImg
        visible: false
    }

    contentItem: OpacityMask {
        id: opacityImg
        anchors.centerIn: parent
        source: colorRect
        maskSource: iconImg
    }
}

