/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
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

import QtQuick 2.13

Rectangle {
    id: root

    enum LineSide {
        Top = 0,
        Right = 1,
        Bottom = 2,
        Left = 3
    }

    property Item target: null
    property int side: LineSide.Bottom

    property QtObject __private: QtObject {
        property bool horizontal: root.side === ThinLine.LineSide.Top || root.side === ThinLine.LineSide.Bottom
    }

    x: side === ThinLine.LineSide.Right ? target.width - width : 0
    y: side === ThinLine.LineSide.Bottom ? target.height - height : 0

    height: __private.horizontal ? Math.floor(Math.max(target.height / 100.0, 1)) : target.height
    width:  __private.horizontal ? target.width  : Math.floor(Math.max(target.width / 100.0, 1))
}
