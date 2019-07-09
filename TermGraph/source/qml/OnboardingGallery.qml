import QtQuick 2.5
import QtQuick.Controls 2.5

import "UIExtensions"

Page {
    id: root

    property StackView mainStack
    property ListModel helpModel: ListModel {
        ListElement { text: "Привет! Спасибо, что установили это приложение. Надеюсь оно сможет быть вам полезным" }
        ListElement { text: "Основная цель приложения - помощь при образовании. И в дальнейшем оно будет развиваться именно в этом ключе" }
        ListElement { text: "Помощь заключается в полу-автоматическом составлении \"Карты терминов\". Если встречали приложения из разряда mind-map - вот очень похоже, если не встречали - по ходу думаю сориентируетесь" }
        ListElement { text: "В общем суть в том чтобы составить \"карту\" изучаемой предметной области с помощью терминов" }
        ListElement { text: "В учебниках термины обычно располагаются от простых к сложным: например \"вершина\" - простой (базовый) термин, \"треугольник\" - составной термин который можно объяснить через 3 вершины" }
        ListElement { text: "Т.е. знание о вершине - самодостаточно. Знание о треугольнике без знания о вершине - довольно бессмысленно" }
        ListElement { text: "Схема работы состоит в следующем. Добавляйте термины и в определении фигурными скобками, вот такими -> {} выделяйте \"базовые\" к ним определения. В общем все" }
        ListElement { text: "Пример: \"Вершина\" - точка в пространстве. \"Треугольник\" - фигура, состоящая из 3х {вершин}, соединенных прямыми." }
        ListElement { text: "По сути нужно просто указать какие термины зависят от других более базовых терминов. Все остальное за вас сделает алгоритм: первыми слева расположит базовые термины и чем правее тем более сложными и составными они будут становиться" }
        ListElement { text: "В итоге получится подобие карты терминов, где слева будет самая база, а справа самая жесть. Лично мне очень помогает структурировать какие то знания по ходу изучения. Становится понятно, что от чего зависит" }
        ListElement { text: "В общем что то вроде блокнота, только со связями и в дальнейшем функционал будет расширяться" }
        ListElement { text: "И + если вы вдруг забросите изучение этой области на пару - тройку недель, когда вы к ней вернетесь, легко сможете \"окинуть ее взглядом\" и вспомнить важные определения" }
        ListElement { text: "Лично мне очень хорошо помогло при изучении клеточной биологии, потому что там много терминов и все их за раз переварить проблематично, а постоянно возвращаться к ним по книге утомительно" }
        ListElement { text: "В общем все. Дальше по подсказкам" }
    }

    function open() {
        mainStack.push(root)
    }

    header: MainHeader {
        id: mainHeader
        titleText: "Справка"

        Component.onCompleted: mainHeader.showArrowIcon()

        onMenuClick: mainStack.pop()
    }

    SwipeView {
        id: swipeView
        currentIndex: 0
        anchors.fill: parent

        property real minWH: Math.min(width, height)

        Repeater {
            model: root.helpModel

            Loader {
                active: SwipeView.isCurrentItem || SwipeView.isNextItem || SwipeView.isPreviousItem
                sourceComponent: OnBoardCard {
                    id: tip
                    baseSize: swipeView.minWH
                    text: model.text
                }
            }
        }
    }

    PageIndicator {
        id: pageIndicator

        count: swipeView.count
        currentIndex: swipeView.currentIndex

        anchors.bottom: swipeView.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
