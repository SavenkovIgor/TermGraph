import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Window 2.13

Drawer {
    id: drawer

    property alias maxWidth: groupListView.maxWidth

    edge: Qt.RightEdge
    onOpened: groupListView.forceActiveFocus()

    ListView {
        id: groupListView
        anchors.fill: parent

        property real maxWidth: 0

        model: groupsManager.allUuidSorted

        Rectangle {
            anchors.fill: parent
            color: appColors.baseLight
            z: -1
        }

        keyNavigationEnabled: true

        delegate: Rectangle {
            id: groupLstDlgt
            anchors.left: parent.left
            anchors.right: parent.right

            color: "transparent"

            height: curText.height

            states: State {
                name: "Current"
                when: groupLstDlgt.ListView.isCurrentItem
                PropertyChanges { target: groupLstDlgt; color: "darkGray" }
            }

            Text {
                id: curText
                padding: 30

                font.weight: Font.Thin
                height: Math.floor( font.pixelSize*2.0 )

                color: appColors.white

                text: groupsManager.getGroupName(modelData)

                onContentWidthChanged: {
                    if (contentWidth > groupListView.maxWidth) {
                        groupListView.maxWidth = contentWidth + padding * 2
                    }
                }

                font.pixelSize: mainObj.getUiElementSize("text") * Screen.pixelDensity
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }

            ThinLine {
                target: parent
                side: ThinLine.LineSide.Bottom
                color: appColors.white
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    drawer.close()
                    sceneObj.showGroup(modelData)
                }
            }
        }
    }
}
