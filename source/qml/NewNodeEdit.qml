import QtQuick 2.5
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3

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
                addNode.act()

    }

    function moveBack() {
        stackView.popItem()
    }

    function prepare() {
        if( changeN.text == "" ) {
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
        rootNode.checked = sceneObj.getCurrNodeIsRoot()
    }

    function clear() {
        changeN.text     = ""
        termName.text    = ""
        termForms.text   = ""
        termDefin.text   = ""
        termDescr.text   = ""
        termExampl.text  = ""
        rootNode.checked = ""
    }


    MyRoundButton {
        id: makeBrackets

        anchors.right: parent.right
        anchors.top: parent.top

        visible: false

        onClicked: {
            termDefin.makeFocusInField()
            termDefin.text = sceneObj.setTagsAtPos( termDefin.getCursPosition(), termDefin.text )
        }
        Component.onCompleted: loadIcon("qrc:/icons/code")
    }

    ScrollView {
        anchors.fill: parent

        Column {
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
                    text: mainObj.changeNum
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

            CheckBox {
                id: rootNode
                objectName: "rootNode"

                text: "Корневая вершина"
                font.pixelSize: mainObj.getUiElementSize("inputText")*Screen.pixelDensity
                checked: false
            }

            RowLayout {
                width: parent.width
                Label {
                    id: grpLabel
                    text : "Группа"
                    font.pixelSize: mainObj.getUiElementSize("inputText")*Screen.pixelDensity
                }

                MyComboBox {
                    id: nodeGroup
                    model: mainObj.groupsList;
                    currentIndex: mainObj.currGroupNum

                    Layout.fillWidth: true

                }
            }

            MySquareButton {
                id: addNode
                text : "Добавить/Изменить"

                function act() {
                    mainObj.setChangeNum(changeN.text)
                    mainObj.addNewNode(
                                termName.text,
                                termForms.text,
                                termDefin.text,
                                termDescr.text,
                                termExampl.text,
                                rootNode.checked,
                                nodeGroup.currentText
                                )
                    moveBack()
                }
                onClicked: act()
            }

            //            Item{
            //                Layout.fillHeight: true
            //            }
        }
    }
}
