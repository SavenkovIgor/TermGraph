import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Window 2.13
import QtQuick.Dialogs 1.3

import "../UIExtensions"
import "../Js/iconpath.js" as IconPath

Page {
    id: root

    header: MainHeader {

        id: mainHeader
        title: "Выберите группу для повторения"

        Component.onCompleted: mainHeader.showArrowIcon()

        onMenuClick: root.StackView.view.pop()
    }

    background: Rectangle { color: appColors.base }

    MyRoundButton {
        id: addGroupBtn
        anchors { right: parent.right; bottom: parent.bottom; margins: width / 2; }
        icon.source: IconPath.rightArrow
    }

    contentItem: ListView {
        id: groupsList
        model: groupsManager.allUuidSorted
        keyNavigationEnabled: true

        highlight: Rectangle {
            width: 200; height: 20
            color: appColors.baseLight2
            y: groupsList.currentItem.y;
        }

        delegate: Rectangle {
            id: lstDlgt
//            border.color: "lightGray"
//            border.width: 1
            anchors.left: parent.left
            anchors.right: parent.right

            property alias text: grpName.text
            property string groupUuid: modelData

            ThinLine {
                target: lstDlgt
                side: ThinLine.LineSide.Bottom
            }

            height: grCol.height
            states: State {
                name: "Current"
                when: lstDlgt.ListView.isCurrentItem
                PropertyChanges { target: lstDlgt; color: appColors.baseLight2 }
            }

            color: "transparent"

            Column {
                id: grCol

                Text {
                    id: grpName
                    topPadding: font.pixelSize / 2
                    leftPadding: font.pixelSize
                    bottomPadding: font.pixelSize / 3

                    font.weight: Font.Medium

                    color: appColors.white

                    text: groupsManager.getGroupName(modelData)
                    font.pixelSize: mainObj.getUiElementSize("text") * Screen.pixelDensity
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }

                SmallInfoText {
                    description: "Uuid"
                    label: modelData
                }

                SmallInfoText {
                    description: "Last editing time"
                    label: groupsManager.getLastEditString(modelData)
                }

                SmallInfoText {
                    description: "Node count"
                    label: groupsManager.getNodesCount(modelData)

                    bottomPadding: grpName.font.pixelSize / 2
                }
            }

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    lstDlgt.forceActiveFocus()
                    onClicked: lstDlgt.ListView.view.currentIndex = index
                }
            }
        }
    }
}
