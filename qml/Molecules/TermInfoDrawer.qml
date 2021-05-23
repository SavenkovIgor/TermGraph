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
import QtQuick.Layouts 1.15

import Api 1.0

import StyleInfo 1.0
import Atoms 1.0 as A

A.DrawerPage {
    id: root

    property LinksHardeningManager linksManager: LinksHardeningManager { }

    property var currentNode

    signal openInfoPage
    signal editNode
    signal openWarningPopup

    topPadding:    Sizes.baseR50
    leftPadding:   Sizes.baseR50
    rightPadding:  Sizes.baseR50
    bottomPadding: Sizes.baseR50

    implicitHeight: topPadding + implicitContentHeight + bottomPadding

    contentItem:  A.FlickableColumn {
        spacing: 14

        RowLayout {
            LabelPair {
                name: "Название:"
                text: currentNode ? currentNode.term : ""
                visible: text !== ""
                Layout.fillWidth: true
            }

            A.RoundButton {
                bgColor: Colors.yellow
                visible: TagProcessor.hasSoftLinks(currentNode ? currentNode.definition : "")
                action: Action {
                    icon.source: IconPath.warning
                    onTriggered: root.openWarningPopup()
                }
            }

            A.RoundButton {
                bgColor: Colors.whiteDisabled
                action: Action {
                    icon.source: IconPath.info
                    onTriggered: root.openInfoPage()
                }
            }

            A.RoundButton {
                bgColor: Colors.whiteDisabled
                action: Action {
                    icon.source: IconPath.pencil
                    onTriggered: root.editNode()
                }
            }
        }

        LabelPair {
            name: "Определение:"
            text: TagProcessor.decorateTags(Scene.currNodeNameAndDefinition)
            visible: text !== ""
            Layout.fillWidth: true
        }

        LabelPair {
            name: "Описание:"
            text: currentNode ? currentNode.description : ""
            visible: text !== ""
            Layout.fillWidth: true
        }

        LabelPair {
            name: "Пример:"
            text: currentNode ? currentNode.examples : ""
            visible: text !== ""
            Layout.fillWidth: true
        }
    }
}
