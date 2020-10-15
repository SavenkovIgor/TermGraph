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

import "../Atoms" as A

import StyleInfo 1.0
import Helpers 1.0

A.DrawerPage {
    id: root

    property LinksHardeningManager linksManager: LinksHardeningManager { }
    property var currentNode

    signal showTermUuid(string uuid)

    onCurrentNodeChanged: {
        linksManager.setGroup(scene.currentGroup);
        linksManager.setTerm(scene.currentNode);
    }

    contentItem: ColumnLayout {

        RowLayout {
            id: row

            Layout.fillWidth: true

            TextArea {
                Layout.fillWidth: true
                height: 50
                text: linksManager.currentLinkText
                readOnly: true
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

        ListView { // uuid variants
            model: linksManager
            height: 200
            Layout.fillWidth: true
            Layout.fillHeight: true

            boundsBehavior: ListView.StopAtBounds
            clip: true
            delegate: RadioButton {
                id: delegateItem

                required property string termUuid
                required property string termText
                required property int termDistance

                text: termText + "(" + termDistance + ")"

                width: ListView.view.width

                onClicked: {
                    root.showTermUuid(termUuid)
                }

//                RowLayout {


//                    Text {
//                        Layout.fillWidth: true
//                    }

//                    A.RoundButton {
//                        size: (Sizes.baseR25 + Sizes.base) / 2
//                        action: Action {
//                            icon.source: IconPath.eye
                            //                    onTriggered:
//                        }
//                    }

//                    A.RoundButton {
//                        size: (Sizes.baseR25 + Sizes.base) / 2
//                        action: Action {
//                            icon.source: IconPath.check
                            //                    onTriggered:
//                        }
//                    }
//                }
            }
        }
    }
}
