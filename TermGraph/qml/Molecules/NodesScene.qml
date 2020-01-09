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
import QtQuick.Controls 2.13

import "../Atoms" as A

import "../Js/Fonts.js" as Fonts
import "../Js/Colors.js" as Colors
import "../Js/NodePaint.js" as JsPaint

import Notification 1.0

Control {

    height: scene.sceneRect.height
    width: scene.sceneRect.width

    background: Rectangle { color: Colors.base }

    Component.onCompleted: {
        // Try to show first group
        const groupsUuids = groupsManager.allUuidSorted;

        if (groupsUuids.length !== 0) {
            scene.currentGroup = groupsUuids[0];
        }
    }

    contentItem: Item {
        Canvas {
            id: lineCanvas
            anchors.fill: parent

            renderStrategy: Canvas.Cooperative
            renderTarget: Canvas.Image

            Connections {
                target: paintManager
                onPaintGroupQueue: lineCanvas.requestPaint()
                onPaintNodeQueue:  lineCanvas.requestPaint()
            }

            onPaint: {
                paintManager.setPaintInProcessFlag(true);
                const ctx = lineCanvas.getContext('2d');

                paintAll(ctx);

                paintManager.setPaintInProcessFlag(false);
            }

            function paintAll(ctx)
            {
                clearRects(ctx);
                paintGroupRects(ctx);
                paintGroupNames(ctx);
                paintEdges(ctx);
            }

            function clearRects(ctx)
            {
                while (true) {
                    if (paintManager.clearQueueEmpty())
                        break;

                    const rect = paintManager.currentClearRect();
                    JsPaint.clearRect(ctx, rect, 0);

                    paintManager.nextClearRect();
                }
            }

            function paintGroupRects(ctx)
            {
                while (true) {
                    if (paintManager.groupRectQueueEmpty())
                        break;

                    const groupRect = paintManager.currentGroupRect();
                    // JsPaint.clearRect(ctx, groupRect, 2)
                    JsPaint.paintRect(ctx, groupRect, "#FFFFFF");

                    paintManager.nextGroupRect();
                }
            }

            function paintGroupNames(ctx)
            {
                while (true) {
                    if (paintManager.groupNamesQueueEmpty())
                        break;

                    const groupName = paintManager.currentGroupName();
                    const groupNamePos = paintManager.currentGroupNamePos();
                    JsPaint.paintGroupName(ctx, groupName, groupNamePos);

                    paintManager.nextGroupName();
                }
            }

            function paintEdges(ctx)
            {
                JsPaint.prepareEdge(ctx);

                while (true) {
                    if (paintManager.edgeQueueEmpty())
                        break;

                    const pt1 = paintManager.currentFirstEdgePoint();
                    const pt2 = paintManager.currentLastEdgePoint();
                    const col = paintManager.getEdgeColor();

                    ctx.strokeStyle = col;
                    ctx.beginPath();
                    JsPaint.drawBLine(ctx, pt1, pt2, col);
                    ctx.stroke();

                    paintManager.nextEdge();
                }
            }
        }

        Repeater {
            model: scene.nodes

            delegate: A.Term {
                rect: modelData.rect
                radius: modelData.radius
                color: modelData.color
                text: modelData.term
            }
        }
    }
}
