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

import Molecules 1.0 as M

M.Page {

    readonly property int s1: 128
    readonly property int s2: 256
    readonly property int s3: 512

    contentItem: ScrollView {
        Column {
            Button {
                text: "Сохранить иконки"
                onClicked: {
                    saveIcon(icon1);
                    saveIcon(icon2);
                    saveIcon(icon3);
                }

                function saveIcon(icon) {
                    icon.grabToImage(function(result){
                        const name = "appIcon" + icon.width + "x" + icon.height + ".png";
                        if (result.saveToFile(name)) {
                            print(name, "save success");
                        } else {
                            print(name, "save fail");
                        }
                    });
                }
            }

            M.AppIcon { id: icon1; size: s1; }
            M.AppIcon { id: icon2; size: s2; }
            M.AppIcon { id: icon3; size: s3; }
        }
    }
}
