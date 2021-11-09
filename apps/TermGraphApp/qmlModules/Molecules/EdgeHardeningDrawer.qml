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

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Api

import Atoms as A
import Molecules as M
import StyleInfo

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
        linksManager.setGroup(Scene.currentGroup);
        linksManager.setTerm(Scene.currentNode);
    }

    contentItem: ColumnLayout {

        RowLayout {
            Layout.fillWidth: true

            Text {
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignLeft
                padding: Sizes.baseR50
                topPadding: Sizes.baseR75
                bottomPadding: Sizes.baseR75

                color: Colors.white
                font: Fonts.setWeight(Fonts.capitalText, Font.DemiBold)

                text: "Фиксация ссылок для термина: '" + root.currentTerm + "'"
            }

            A.RoundButton {
                action: Action {
                    icon.source: IconPath.check
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

                        const success = GroupsManager.updateNode(data);

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
                    icon.source: IconPath.chevronLeft
                    onTriggered: linksManager.prev()
                }
            }
            A.RoundButton {
                enabled: linksManager.canMoveNext
                action: Action {
                    icon.source: IconPath.chevronRight
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
                    color: Colors.white
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
