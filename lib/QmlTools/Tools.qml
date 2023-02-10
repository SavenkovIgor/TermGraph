// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

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
