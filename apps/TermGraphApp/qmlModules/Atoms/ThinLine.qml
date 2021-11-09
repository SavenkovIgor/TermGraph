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
