// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

export module Enums.CoordType;

export enum class CoordType {
    zeroPoint, // Rect located in zero point
    local,     // Local coordinate system
    scene      // Scene coordinate system
};
