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
            return Theme.color.edge.selected;

        if (edgeSelection === EdgeSelection.Forward)
            return Theme.color.edge.selectedAlt;

        switch (edgeType) {
        case EdgeType.Standart:       return Theme.color.edge.standard;
        case EdgeType.Termin:         return Theme.color.edge.termin;
        case EdgeType.TerminHardLink: return Theme.color.edge.terminHardLink;
        case EdgeType.Description:    return Theme.color.edge.description;
        case EdgeType.Broken:         return Theme.color.edge.broken;
        case EdgeType.Redundant:      return Theme.color.edge.standard;
        }

        return Theme.color.edge.standard;
    }

    fillColor: "transparent"

    PathCubic {
        x: root.pt2.x; y: root.pt2.y;
        control1X: b1.x; control1Y: b1.y;
        control2X: b2.x; control2Y: b2.y;
    }
}
