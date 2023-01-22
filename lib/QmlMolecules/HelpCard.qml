// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T

import Theme

T.Control {
    id: root

    property alias text: tipTitle.text
    property real minSizing: Math.min(width, height);

    background: Rectangle { color: Theme.color.base0; }

    contentItem: Rectangle {
        property real marginVal: root.minSizing * 0.05

        anchors { fill: parent; margins: marginVal; }

        radius: root.minSizing * 0.08
        color: Theme.color.base3

        Item {
            id: item
            anchors {
                fill: parent
                leftMargin: parent.width * 0.075
                rightMargin: parent.width * 0.075
                topMargin: parent.height * 0.075
                bottomMargin: parent.height * 0.075
            }

            Flickable {
                id: flick
                anchors.fill: parent

                clip: true
                ScrollBar.vertical: ScrollBar {
                    policy: flick.height < tipTitle.height ? ScrollBar.AlwaysOn : ScrollBar.AsNeeded
                }

                contentWidth: tipTitle.width
                contentHeight: tipTitle.height * 1.05

                boundsBehavior: Flickable.StopAtBounds

                TextEdit {
                    id: tipTitle

                    width: flick.width

                    textFormat: TextEdit.RichText
                    wrapMode: TextEdit.WordWrap
                    readOnly: true

                    font: Theme.font.h3
                    color: "white"
                }
            }

            Rectangle {
                anchors { left: flick.left; bottom: flick.bottom; }

                width: flick.width
                height: flick.height * 0.05

                gradient: Gradient {
                    GradientStop { position: 0.0; color: "transparent"; }
                    GradientStop { position: 1.0; color: Theme.color.base3; }
                }
            }
        }
    }
}
