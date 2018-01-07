import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Window 2.3

//ComboBox {
//    id: box

//    width: contentItem.width
//    height: contentItem.height
//    font.pixelSize: mainObj.getUiElementSize("combo")*Screen.pixelDensity;
//    width: 500
//}

ComboBox {
    id: control
    font.pixelSize: mainObj.getUiElementSize("combo")*Screen.pixelDensity;


//    indicator: Canvas {
//        id: canvas
//        x: control.width - width - control.rightPadding
//        y: control.topPadding + (control.availableHeight - height) / 2
//        width: 12
//        height: 8
//        contextType: "2d"

//        Connections {
//            target: control
//            onPressedChanged: canvas.requestPaint()
//        }

//        onPaint: {
//            context.reset();
//            context.moveTo(0, 0);
//            context.lineTo(width, 0);
//            context.lineTo(width / 2, height);
//            context.closePath();
//            context.fillStyle = control.pressed ? "#17a81a" : "#21be2b";
//            context.fill();
//        }
//    }

    contentItem: Text {
        //              leftPadding: 0
        //              rightPadding: control.indicator.width + control.spacing
        //              anchors.left: parent.left + 20
        //              anchors.right: parent.right - 20

        id: contItem

        height: contentHeight * 1.8

        text: control.displayText
        font: control.font
//        color: control.pressed ? "#17a81a" : "#21be2b"
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: contItem.width+35
        implicitHeight: contItem.height
        border.color: "#000000"
        border.width: 1
    }

    popup: Popup {
        y: control.height - 1
        width: control.width
        implicitHeight: contentItem.implicitHeight
        padding: 1

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex

            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            border.color: "#000000"
            radius: 2
        }
    }

    //Item in the popup
    delegate: ItemDelegate {
        width: control.width
        contentItem: Text {
            text: modelData
//            color: "#21be2b"
            font: control.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }

        highlighted: control.highlightedIndex === index
    }

}

