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

pragma Singleton

import QtQml

QtObject {
    function clamp(num, min, max) {
        return num <= min ? min : num >= max ? max : num;
    }

    function clampPoint(point, size) {
        point.x = clamp(point.x, 0, size.width);
        point.y = clamp(point.y, 0, size.height);
        return point;
    }

    function scalePoint(point, scale) {
        point.x *= scale;
        point.y *= scale;
        return point;
    }

    function scaleSize(size, scale) {
        return Qt.size(size.width * scale, size.height * scale);
    }
}
