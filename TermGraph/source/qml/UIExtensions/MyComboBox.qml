import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

ComboBox {
    id: control
    font.pixelSize: mainObj.getUiElementSize("combo")*Screen.pixelDensity;

    contentItem: Text {

        id: contItem

        height: contentHeight * 1.8

        text: groupsManager.getGroupName(control.displayText)
        font: control.font
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        width: contItem.width+35
        height: contItem.height
        border.color: appColors.black
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
            border.color: appColors.black
            radius: 2
        }
    }

    //Item in the popup
    delegate: ItemDelegate {
        width: control.width
        contentItem: Text {
            text: groupsManager.getGroupName(modelData)
            font: control.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }

        highlighted: control.highlightedIndex === index
    }

}
