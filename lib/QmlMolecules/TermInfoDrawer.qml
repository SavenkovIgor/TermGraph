// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Api

import Theme
import Atoms as A

A.DrawerPage {
    id: root

    property LinksHardeningManager linksManager: LinksHardeningManager { }

    property var currentNode

    signal openInfoPage
    signal editNode
    signal openWarningPopup

    topPadding:    Sizes.baseX2
    leftPadding:   Sizes.baseX2
    rightPadding:  Sizes.baseX2
    bottomPadding: Sizes.baseX2

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
                bgColor: Theme.color.warning
                visible: LinkTools.hasSoftLinks(currentNode ? currentNode.definition : "") && !Api.readonlyMode
                action: Action {
                    enabled: !Api.readonlyMode
                    icon.source: Theme.icon.warning
                    onTriggered: root.openWarningPopup()
                }
            }

            A.RoundButton {
                bgColor: Theme.color.base4
                action: Action {
                    icon.source: Theme.icon.info
                    onTriggered: root.openInfoPage()
                }
            }

            A.RoundButton {
                visible: !Api.readonlyMode
                bgColor: Theme.color.base4
                action: Action {
                    enabled: !Api.readonlyMode
                    icon.source: Theme.icon.pencil
                    onTriggered: root.editNode()
                }
            }
        }

        LabelPair {
            name: "Определение:"
            text: LinkTools.decorate(Api.scene.currNodeNameAndDefinition)
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
