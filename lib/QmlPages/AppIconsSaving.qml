// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import Molecules as M

M.Page {

    readonly property int s1: 128
    readonly property int s2: 256
    readonly property int s3: 512

    contentItem: ScrollView {
        Row {
            M.AppIcon { id: icon1; size: s1; }
            M.AppIcon { id: icon2; size: s2; }
            M.AppIcon { id: icon3; size: s3; }
        }
    }

    Button {
        anchors.bottom: parent.bottom
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
}
