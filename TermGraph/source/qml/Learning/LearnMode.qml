import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Window 2.13
import QtQuick.Dialogs 1.3

import "../UIExtensions"
import "../JsExtensions/iconpath.js" as IconPath

Page {
    id: root

    header: MainHeader {

        id: mainHeader
        title: "Режим изучения"

        Component.onCompleted: mainHeader.showArrowIcon()

        onMenuClick: root.StackView.view.pop()
    }

    background: Rectangle { color: appColors.base }

    MyRoundButton {
        id: addGroupBtn
        anchors { right: parent.right; bottom: parent.bottom; }
        icon.source: IconPath.rightArrow
    }

    contentItem: ListView {
        id: groupsList
        model: ["Названия вершин"]
        keyNavigationEnabled: true

        highlight: Rectangle {
            width: 200; height: 20
            color: appColors.baseLight2
            y: groupsList.currentItem.y;
        }

        delegate: Control {

            property string groupUuid: modelData

            anchors.fill: parent

            background: Rectangle {
                color: "transparent"
                ThinLine { target: parent; side: ThinLine.LineSide.Bottom; }
            }

            contentItem: Text {
                anchors.fill: parent
                leftPadding: font.pixelSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft

                text: modelData
                color: appColors.white
                font {
                    weight: Font.Thin
                    pixelSize: mainObj.getUiElementSize("capitalText") * Screen.pixelDensity
                }

                MouseArea{ anchors.fill: parent }  // TODO: Make action here
            }
        }
    }
}
