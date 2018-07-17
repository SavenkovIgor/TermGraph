import QtQuick 2.5
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import QtQuick.Dialogs 1.2

import "UIExtensions"

Item {
    id: newNodeEdit
    objectName: "newNodeEdit"
    anchors.fill: parent

    Keys.onEscapePressed: {
        moveBack()
    }

    Keys.onPressed: {
        if( event.modifiers === Qt.ControlModifier )
            if( event.key === Qt.Key_Return || event.key === Qt.Key_Enter )
                addNode.addOrChangeNode()
    }

    function moveBack() {
        stackView.popItem()
    }

    function prepare(nodeUid) {
        changeN.text = nodeUid
        if( changeN.text === "" ) {
            clear()
            changeLbl.visible = false
            changeN.visible = false
            addNode.text = "Добавить";
        } else {
            fillInfo()
            changeLbl.visible = true
            changeN.visible = true
            addNode.text = "Изменить";
        }
        termName.forceActiveFocus()
    }

    function fillInfo() {
        termName.text    = sceneObj.getCurrNodeName()
        termForms.text   = sceneObj.getCurrNodeForms()
        termDefin.text   = sceneObj.getCurrNodeDefinition()
        termDescr.text   = sceneObj.getCurrNodeDescription()
        termExampl.text  = sceneObj.getCurrNodeExamples()
        nodeGroup.selectElement(sceneObj.getCurrNodeGroupName())
    }

    function clear() {
        changeN.text     = ""
        termName.text    = ""
        termForms.text   = ""
        termDefin.text   = ""
        termDescr.text   = ""
        termExampl.text  = ""
    }


    MyRoundButton {
        id: makeBrackets

        anchors.right: parent.right
        anchors.top: parent.top

        visible: false

        onClicked: {
            termDefin.makeFocusInField()
            termDefin.text = tagProcessor.addTagInPosition( termDefin.getCursPosition(), termDefin.text )
        }
        Component.onCompleted: loadIcon("qrc:/icons/code")
    }

    ScrollView {
        anchors.fill: parent

        Column {
            id: viewColumn

            anchors.margins: 12
            width: newNodeEdit.width

            spacing: mainObj.getUiElementSize("colSpace")*Screen.pixelDensity

            Row {
                Label {
                    id : changeLbl
                    text: "Изменить: "
                    font.pixelSize: mainObj.getUiElementSize("inputText")*Screen.pixelDensity
                }

                Label {
                    id : changeN
//                    text: mainObj.changeNum
                    font.pixelSize: mainObj.getUiElementSize("inputText")*Screen.pixelDensity
                    onTextChanged: {
                        fillInfo()
                    }
                }
            }

            MyTextField {
                id: termName
                labelText: "Название:"
                placeholderText: "[Термин]"
            }

            MyTextField {
                id : termForms
                labelText : "Грамматические формы:"
                placeholderText: "[Альтернативные окончания термина, как слОва]"
            }

            MyTextArea {
                labelText: "-это"
                id: termDefin
                placeholderText: "[Определение. Ссылки формируются с помощью фигурных скобок {} ]"
                onTxtFocusChanged: {
                    if( txtFocus )
                        makeBrackets.visible = true
                    else
                        makeBrackets.visible = false
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
                width: parent.width
                Label {
                    id: grpLabel
                    text : "Группа"
                    font.pixelSize: mainObj.getUiElementSize("inputText")*Screen.pixelDensity
                }

                Connections {
                    target: groupsManager
                    onGroupsListChanged: {
                        nodeGroup.refreshModel()
                    }
                }

                MyComboBox {
                    id: nodeGroup
                    model: groupsManager.getAllGroupsNames()

                    function refreshModel() {
                        model = groupsManager.getAllGroupsNames()
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

            MySquareButton {
                id: addNode
                text : "Добавить/Изменить"

                onClicked: addOrChangeNode()
                function addOrChangeNode() {
                    if(termName.text == "") {
                        emptyNodeNameDelDialog.visible = true
                        return
                    }

                    if( changeN.text == "" ) {
                        nodesManager.addNewNode(
                                    termName.text,
                                    termForms.text,
                                    termDefin.text,
                                    termDescr.text,
                                    termExampl.text,
                                    nodeGroup.currentText
                                    )
                    } else {
                        nodesManager.changeNode(
                                    changeN.text,
                                    termName.text,
                                    termForms.text,
                                    termDefin.text,
                                    termDescr.text,
                                    termExampl.text,
                                    nodeGroup.currentText
                                    )
                    }

                    moveBack()
                }
            }

            //            Item{
            //                Layout.fillHeight: true
            //            }
        }
    }
}
