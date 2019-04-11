import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.3

Drawer {
    id: drawer

    property alias maxWidth: groupListView.maxWidth

    edge: Qt.RightEdge
    onOpened: groupListView.forceActiveFocus()

    function refreshModel() {
        groupListView.refreshModel()
    }

    ListView {
        id: groupListView
        anchors.fill: parent

        property real maxWidth: 0

        model: groupsManager.getAllUuidStringsSortedByLastEdit()

        function refreshModel() {
            model = groupsManager.getAllUuidStringsSortedByLastEdit()
        }

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

            Rectangle {
                x: Math.max((parent.width - width) / 2, 1)
                y: parent.height - height
                height: Math.max(parent.height / 100, 1)
                width: parent.width * 0.95
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
