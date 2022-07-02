// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick

MouseArea {
    readonly property point pos: Qt.point(mouseX, mouseY)

    hoverEnabled: true

    function posMappedTo(target) {
        return mapToItem(target, pos.x, pos.y);
    }
}
