// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick as Q

Q.MouseArea {
    readonly property point pos: Qt.point(mouseX, mouseY)

    hoverEnabled: true

    function posMappedTo(target) {
        return mapToItem(target, pos.x, pos.y);
    }
}
