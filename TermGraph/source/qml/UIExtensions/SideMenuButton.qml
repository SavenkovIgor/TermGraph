import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Window 2.2

Button {

    property alias label: buttonLabel.text

    flat: true
    //height: Math.floor( font.pixelSize * 2.7 )

    function loadIcon( nameUrl ) {
        var sz = height - ( 2*padding ) //need icon width

        var prefix = getPrefixForSize(sz)

        image.width  = Math.floor(sz)
        image.height = Math.floor(sz)

//        console.log(iconImg.width)
//        console.log(iconImg.height)

        image.source = nameUrl + prefix + ".png"
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

    contentItem: Item {

        Image {
            id: image
            anchors {
//                left: parent.left
//                top: parent.top
//                bottom: parent.bottom
            }
        }

        Text {
            id: buttonLabel

            anchors {
                verticalCenter: image.verticalCenter
                left: image.right
                leftMargin: 10
//                right: parent.right
            }

            font.weight: Font.Medium
            font.pixelSize: mainObj.getUiElementSize("capitalText") * Screen.pixelDensity
        }

        Component.onCompleted: {
            width: image.width + buttonLabel.width
            height: image.height
        }
    }
}
