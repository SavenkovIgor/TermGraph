// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick

Rectangle {
    enum LineSide { Top = 0, Right, Bottom, Left }

    property Item target: parent
    property int side: -1  // Line side
    property int lineWidth: Math.floor( Math.max( (isHorizontal ? target.height : target.width) / 100.0, 1) )

    readonly property bool isHorizontal: side === ThinLine.LineSide.Top || side === ThinLine.LineSide.Bottom

    x: side === ThinLine.LineSide.Right ? target.width  - lineWidth : 0
    y: side === ThinLine.LineSide.Bottom ? target.height - lineWidth : 0

    height: isHorizontal ? lineWidth : target.height
    width: isHorizontal ? target.width : lineWidth
}
