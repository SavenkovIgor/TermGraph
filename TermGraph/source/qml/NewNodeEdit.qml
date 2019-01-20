import QtQuick 2.5
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import QtQuick.Dialogs 1.2

import "UIExtensions"

Page {

    id: newNodeEdit

    property StackView mainStack
    property string changingNodeUuid

    header: MainHeader {

        id: mainHeader
        titleText: ""

        Component.onCompleted: {
            mainHeader.showCheckButton()
            mainHeader.showArrowIcon()
        }

        onMenuClick: mainStack.pop()
        onCheckClick: newNodeEdit.addOrChangeNode()
    }

    Keys.onEscapePressed: {
        exitFromThisPage()
    }

    Keys.onPressed: {
        if (event.modifiers === Qt.ControlModifier) {
            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                newNodeEdit.addOrChangeNode()
            }
        }
    }

    function exitFromThisPage() {
        mainStack.pop()
    }

    function prepare(nodeUuid) {
        changeNodeRow.text = nodeUuid

        var newNode = changeNodeRow.text === ""  // bool

        changeNodeRow.visible = !newNode
        currentGroupFixedRow.visible = newNode
        currentGroupEditableRow.visible = !newNode

        if (newNode) {
            clear()    
            mainHeader.titleText = "Добавить вершину"
            currentGroupFixedRow.groupUuid = sceneObj.getCurrGroupUuid()
        } else {
            fillInfo()
            mainHeader.titleText = "Изменить вершину"
        }

        termName.forceActiveFocus()
    }

    function fillInfo() {
        termName.text    = sceneObj.getCurrNodeName()
//        termForms.text   = sceneObj.getCurrNodeForms()
        termDefin.text   = sceneObj.getCurrNodeDefinition()
        termDescr.text   = sceneObj.getCurrNodeDescription()
        termExampl.text  = sceneObj.getCurrNodeExamples()
        nodeGroup.selectElement(sceneObj.getCurrNodeGroupName())
    }

    function clear() {
        changeNodeRow.text     = ""
        termName.text    = ""
//        termForms.text   = ""
        termDefin.text   = ""
        termDescr.text   = ""
        termExampl.text  = ""
    }

    function addOrChangeNode() {
        if (termName.text == "") {
            emptyNodeNameDelDialog.visible = true
            return
        }

        var success = true;
        var newNode = changeNodeRow.text === ""  // bool

        if (newNode) {
            success = nodesManager.addNewNode(
                        termName.text,
                        "",
                        termDefin.text,
                        termDescr.text,
                        termExampl.text,
                        currentGroupFixedRow.groupUuid
                        )
        } else {
            success = nodesManager.changeNode(
                        changeNodeRow.text,
                        termName.text,
                        "",
                        termDefin.text,
                        termDescr.text,
                        termExampl.text,
                        nodeGroup.currentText
                        )
        }

        if (success) {
            exitFromThisPage()
        }
    }


    MyRoundButton {
        id: makeTag

        anchors.right: expandTagRight.left
        anchors.top: parent.top

        visible: false

        onClicked: {
            termDefin.makeFocusInField()
            var pos = termDefin.getCursPosition()
            termDefin.text = tagProcessor.addTagInPosition( pos, termDefin.text )
            termDefin.setCursPosition(pos);
        }
        Component.onCompleted: loadIcon("qrc:/icons/code")
    }

    MyRoundButton {
        id: expandTagRight

        anchors.right: parent.right
        anchors.top: parent.top

        visible: false

        onClicked: {
            termDefin.makeFocusInField()
            var pos = termDefin.getCursPosition()
            termDefin.text = tagProcessor.expandRight( pos, termDefin.text )
            termDefin.setCursPosition(pos)
        }
        Component.onCompleted: loadIcon("qrc:/icons/extendRight")
    }

    ScrollView {
        anchors.fill: parent
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        Column {
            id: viewColumn

            anchors.margins: 12
            width: newNodeEdit.width

            spacing: mainObj.getUiElementSize("colSpace")*Screen.pixelDensity

            MyLabelPair {
                id: changeNodeRow
                name: "Изменить вершину с uuid: "
                onTextChanged: fillInfo()
            }

            MyLabelPair {
                id: currentGroupFixedRow
                name: "Текущая группа: "

                property string groupUuid: ""
                onGroupUuidChanged: currentGroupFixedRow.text = groupsManager.getGroupName(groupUuid)
            }

            MyTextField {
                id: termName
                labelText: "Название:"
                placeholderText: "[Термин]"
            }

//            MyTextField {
//                id : termForms
//                labelText : "Грамматические формы:"
//                placeholderText: "[Альтернативные окончания термина, как слОва]"
//            }

            MyTextArea {
                labelText: "-это"
                id: termDefin
                placeholderText: "[Определение. Ссылки формируются с помощью фигурных скобок {} ]"
                onTxtFocusChanged: {
                    var visible = txtFocus
                    expandTagRight.visible = visible
                    makeTag.visible = visible
                }
            }

            MyTextArea {
                labelText : "Описание"
                id: termDescr
                placeholderText: "[Общее словестное описание, пока никак не участвует в логике]"
            }

            MyTextArea {
                labelText: "Примеры"
                id: termExampl
            }

            MyTextField {
                enabled: false
                visible: false

                labelText: "Ссылка на Wiki статью"
                placeholderText: "http://"
            }

            MyTextField {
                enabled: false
                visible: false

                labelText: "Ссылка на Wiki изображение"
                placeholderText: "http://"
            }

            // Здесь есть проблема с anchors
            RowLayout {
                id: currentGroupEditableRow

                width: parent.width
                Label {
                    id: grpLabel
                    text : "Группа"
                    font.pixelSize: mainObj.getUiElementSize("inputText")*Screen.pixelDensity
                }

                Connections {
                    target: groupsManager
                    onGroupsListChanged: nodeGroup.refreshModel()
                }

                Connections {
                    target: nodesManager
                    onNodeChanged: nodeGroup.refreshModel()
                }

                MyComboBox {
                    id: nodeGroup
                    model: groupsManager.getAllUuidStringsSortedByLastEdit()

                    function refreshModel() {
                        model = groupsManager.getAllUuidStringsSortedByLastEdit()
                    }

                    function selectElement(name) {
                        var index = find(name)
                        if(index !== -1) {
                            currentIndex = index
                        }
                    }
                    Layout.fillWidth: true
                }
            }

            MessageDialog {
                id: emptyNodeNameDelDialog

                title: "Невозможно создать вершину"
                text:  "Невозможно создать пустой термин.\nЗаполните поле \"Название\""

                standardButtons: StandardButton.Ok
                icon: StandardIcon.Warning
            }

            //            Item{
            //                Layout.fillHeight: true
            //            }
        }
    }
}
