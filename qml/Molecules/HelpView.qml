/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

import QtQuick 2.15
import QtQuick.Controls 2.15

import Atoms 1.0 as A
import Molecules 1.0 as M
import StyleInfo 1.0

Item {
    id: root

    property alias slideNumber: swipeView.currentIndex
    property alias slideCount: swipeView.count
    property bool lastSlide: slideNumber === (slideCount - 1)

    property var helpModel: [
        // Slide 1
        "Привет!<br/>
        <br/>
        Спасибо, что установили приложение TermGraph.<br/>
        Надеюсь, что оно будет вам полезно.<br/>
        <br/>
        <b>Что такое TermGraph?</b><br/>
        <br/>
        Цель приложения - помочь в структурировании ваших знаний.<br/>
        TermGraph работает по принципу, mind-map. Оно выстраивает термины в иерархическом порядке и
        отображает связи между ними.<br/>
        <br/>
        В результате вы получите карту терминов, которую можно использовать так:<br/>
        <ul>
          <li>можно понять, сколько материала уже освоено</li>
          <li>оценить, сколько еще примерно предстоит изучить</li>
          <li>получить представление о материале в целом (даже при обучении урывками)</li>
        </ul>",

        // Slide 2
        // TODO: Добавь тезаурус
        "<b>Как работать с приложением:</b><br/>
        <br/>
        1) Содайте группу терминов и придумайте ей емкое название, например <i>\"Геометрия\"</i>.<br/>
        2) Добавьте в эту группу пару терминов (кнопка + на основном экране).<br/>
        Например, возьмем термины <i>\"точки\"</i> и <i>\"прямой\"</i>:<br/>
        <br/>
        <img width=" + getWidth() + " src=\"qrc:/point.png\"> <br/>
        <br/>
        <img width=" + getWidth() + " src=\"qrc:/line.png\"> <br/>
        <br/>
        Получится такая схема: <br/>
        <br/>
        <img width=" + getWidth() + " src=\"qrc:/graph1.png\"> <br/>
        <br/>
        3) Теперь создадим связь между терминами.<br/>
        Добавим термин <i>\"Отрезок\"</i>, который мы определим через <i>\"Точку\"</i> и <i>\"Прямую\"</i>:<br/>
        <br/>
        <img width=" + getWidth() + " src=\"qrc:/lineSegment.png\"> <br/>
        <br/>
        И еще добавим <i>\"Треугольник\"</i>, просто для красоты<br/>
        <br/>
        <img width=" + getWidth() + " src=\"qrc:/triangle.png\"><br/>
        <br/>
        И в результате мы получим такую схему:<br/>
        <br/>
        <img width=" + getWidth() + " src=\"qrc:/graph3.png\"> <br/>
        <br/>
        В определении обязательно заключайте базовые термины в фигурные скобки {}, это позволяет приложению
        создать связь между ними в группе. Скобки можно проставить нажатием круглой белой кнопки.<br/>
        <br/>
        Термины будут выстраиваться по горизонтали, постепенно усложняясь слева-направо -
        от простых, до специализированных.<br/>
        <br/>
        <b>Цветовое кодирование на схеме</b><br/>
        Цвета терминов:
        <ul>
          <li><i>синий</i> - несвязанный термин</li>
          <li><i>красный</i> - корневой термин (который не зависит ни от каких других терминов)</li>
          <li><i>зеленый</i> - обычный термин в схеме</li>
        </ul>
        Цвета связей:<br/>
        По умолчанию все связи - синие, но при выделении термина голубыми связями будут подсвечены
        все его базовые термины, а красными связями - те, для которых он сам базовый.<br/>
        Циклические связи терминов будут постоянно выделены красным - это является ошибкой,
        циклы мешают корректному построению схемы, их следует удалять.
        <br/>
        Итоговая схема в идеале должна давать очень точный ответ на такие вопросы:<br/>
        <i>\"С помощью каких терминов можно наиболее просто объяснить этот термин?\"</i><br/>
        <i>\"Сколько других терминов нужно еще узнать, чтобы понять смысл этого термина?\"</i><br/>",

        // Slide 3
        "Замечания:<br/>
        <br/>
        Осторожно! Может вызывать иллюзию знания. Помните, что реальное знание включает в себя не только
        владение терминологией, но и способность решать типовые задачи, делать правильные предсказания
        развития событий в типовых ситуациях и пр.<br/>
        <br/>",

        // Slide 4
        "От автора:<br/>
        <br/>
        Лично мне это приложение очень помогает структурировать знания по ходу изучения.
        Использовал при изучении клеточной биологии, потому что там много терминов и все их за раз
        охватить проблематично, а постоянно возвращаться к ним по книге утомительно.<br/>
        <br/>
        Я думаю, что на данный момент это приложение лучше всего подойдет для
        точных наук, в которых много связных друг с другом определений например математика, физика,
        химия и пр.<br/>
        <br/>
        В дальнейшем приложение будет развиваться именно в ключе условного \"блокнота\" который имеет
        представление о твоих знаниях, помогает их хранить, заучивать и всячески поддерживать
        актуальными.<br/>
        <br/>
        В общем все. Удачи в обучении)"
    ]

    function getWidth() {
        return root.width * 0.6;
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent

        Repeater {
            model: root.helpModel
            delegate: M.HelpCard { text: modelData }
        }
    }

    PageIndicator {
        id: pageIndicator

        count: swipeView.count
        currentIndex: swipeView.currentIndex

        anchors.bottom: swipeView.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        delegate: Item {
            width: pin.width * 1.5
            height: pin.height * 1.5

            A.Round {
                id: pin
                diameter: Sizes.baseR75 * 0.75
                anchors.centerIn: parent
                color: index === pageIndicator.currentIndex ? Colors.baseLight2 : Colors.black
            }
        }
    }
}
