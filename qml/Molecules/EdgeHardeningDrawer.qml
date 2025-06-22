// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Api

import Atoms as A
import Molecules as M
import Theme

// TODO: Disable apply button, when nothing changed
// TODO: save selection when switch left|right
// TODO: Cancel selection

A.DrawerPage {
    id: root

    property LinksHardeningManager linksManager: LinksHardeningManager { }
    property var currentNode
    property string currentTerm: currentNode ? currentNode.term : ""

    signal showTermUuid(string uuid)

    onCurrentNodeChanged: {
        linksManager.setGroup(Api.scene.currentGroup);
        linksManager.setTerm(Api.scene.currentNode);
    }

    contentItem: ColumnLayout {

        RowLayout {
            Layout.fillWidth: true

            Text {
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignLeft
                padding: Sizes.baseX2
                topPadding: Sizes.base
                bottomPadding: Sizes.base

                color: Theme.color.base7
                font: Theme.font.setWeight(Theme.font.h3, Font.DemiBold)

                text: "Фиксация ссылок для термина: '" + root.currentTerm + "'"
            }

            A.RoundButton {
                action: Action {
                    icon.source: Theme.icon.check
                    onTriggered: {
                        const hardenedNode = linksManager.appliedReplacement();

                        const data = {
                            uuid: hardenedNode.uuid,
                            term: hardenedNode.term,
                            definition: hardenedNode.definition,
                            description: hardenedNode.description,
                            examples: hardenedNode.examples,
                            wikiUrl: "",
                            wikiImage: "",
                            groupUuid: hardenedNode.groupUuid,
                            lastEdit: ""
                        };

                        const success = Api.groups.updateNode(data);

                        if (success) {
                            root.close();
                        }
                    }
                }
            }
        }

        RowLayout {
            id: row

            Layout.fillWidth: true

            M.TextArea {
                Layout.fillWidth: true
                text: linksManager.definitionWithHighlightedLink
                textFormat: TextArea.RichText
                readOnly: true

                labelText: "Определение:"
            }

            A.RoundButton {
                enabled: linksManager.canMovePrev
                action: Action {
                    icon.source: Theme.icon.chevronLeft
                    onTriggered: linksManager.prev()
                }
            }
            A.RoundButton {
                enabled: linksManager.canMoveNext
                action: Action {
                    icon.source: Theme.icon.chevronRight
                    onTriggered: linksManager.next()
                }
            }
        }

        A.AccentText {
            Layout.fillWidth: true
            text: "Термины, похожие на ссылку " + linksManager.currentLinkText
        }

        ListView { // uuid variants
            id: lView

            model: linksManager ? linksManager : []
            implicitHeight: contentHeight
            Layout.fillWidth: true

            boundsBehavior: ListView.StopAtBounds
            clip: true
            delegate: ItemDelegate {
                id: item

                required property string termUuid
                required property string termText
                required property int termDistance
                required property int index

                contentItem: Text {
                    color: Theme.color.base7
                    text: item.text
                    verticalAlignment: Text.AlignVCenter
                }

                text: termText + "(" + termDistance + ")"

                width: ListView.view.width

                highlighted: ListView.isCurrentItem

                onClicked: {
                    if (!root.linksManager)
                        return;

                    lView.currentIndex = item.index;
                    root.showTermUuid(termUuid);
                    root.linksManager.hardenLink(termUuid);
                }
            }
        }
    }
}
