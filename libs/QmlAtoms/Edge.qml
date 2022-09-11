// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Shapes

import Api
import Theme

ShapePath {
    id: root

    property point pt1: Qt.point(0, 0)
    property point pt2: Qt.point(0, 0)
    property bool isSelected: false
    property int edgeType: EdgeType.Standart
    property int edgeSelection: EdgeSelection.None

    readonly property real halfWidth: (pt2.x - pt1.x) / 2;
    readonly property point b1: Qt.point(pt1.x + halfWidth, pt1.y)
    readonly property point b2: Qt.point(pt2.x - halfWidth, pt2.y)

    startX: pt1.x; startY: pt1.y;

    strokeWidth: Qt.platform.os === "Android" ? 3 : 1
    strokeColor: {
        if (edgeSelection === EdgeSelection.Backward)
            return Colors.edge.selected;

        if (edgeSelection === EdgeSelection.Forward)
            return Colors.edge.selectedAlt;

        switch (edgeType) {
        case EdgeType.Standart:       return Colors.edge.standard;
        case EdgeType.Termin:         return Colors.edge.termin;
        case EdgeType.TerminHardLink: return Colors.edge.terminHardLink;
        case EdgeType.Description:    return Colors.edge.description;
        case EdgeType.Broken:         return Colors.edge.broken;
        case EdgeType.Redundant:      return Colors.edge.standard;
        }

        return Colors.edge.standard;
    }

    fillColor: "transparent"

    PathCubic {
        x: root.pt2.x; y: root.pt2.y;
        control1X: b1.x; control1Y: b1.y;
        control2X: b2.x; control2Y: b2.y;
    }
}
