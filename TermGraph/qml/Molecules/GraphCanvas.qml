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

import "../Js/NodePaint.js" as JsPaint
import "../Js/Colors.js" as Colors

Control {

    height: scene.sceneRect.height
    width: scene.sceneRect.width

    background: Rectangle { color: Colors.base }

    function makeScreenshot() {
        grabToImage(function(result){
            const groupName = scene.currentGroupName;
            const name = mainObj.screenshotFilePath(groupName);
            if (name !== "") {
                result.saveToFile(name);
                Notification.showInfo("Снимок группы создан. Путь:" + name);
            } else {
                Notification.showInfo("Снимок не создан((");
            }
        });
    }

    contentItem: Canvas {
        id: root

        property bool paintGroups: false
        property bool paintNode: false

        renderStrategy: Canvas.Cooperative
        renderTarget: Canvas.Image

        Component.onCompleted: {
            // Try to show first group
            const groupsUuids = groupsManager.allUuidSorted;

            if (groupsUuids.length !== 0) {
                scene.currentGroup = groupsUuids[0];
            }
        }

        Connections {
            target: paintManager
            onPaintGroupQueue: {
                root.paintGroups = true
                root.requestPaint()
            }
            onPaintNodeQueue: {
                root.paintNode = true
                root.requestPaint()
            }
        }

        onPaint: {
            paintManager.setPaintInProcessFlag(true);
            const ctx = root.getContext('2d');

            if (root.paintGroups) {
                paintAll(ctx);
                root.paintGroups = false;
            }

            if (root.paintNode) {
                paintNodes(ctx);
                root.paintNode = false;
            }

            paintManager.setPaintInProcessFlag(false);
        }

        function paintAll(ctx)
        {
            clearRects(ctx);
            paintGroupRects(ctx);
            paintGroupNames(ctx);
            paintEdges(ctx);
            paintNodes(ctx);
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

        function paintNodes(ctx)
        {
            JsPaint.prepareRects(ctx);

            while (true) {
                if (paintManager.nodeQueueEmpty())
                    break;

                const rect = paintManager.currentNodeRect();
                const color = paintManager.currentNodeColor();
                const radius = paintManager.currentNodeRadius();

                JsPaint.paintRoundedRect(ctx, rect, color, radius);

                const center = paintManager.currentNodeCenter();
                const text = paintManager.currentNodeText();

                JsPaint.paintTextWithSplit(ctx, text, center, rect);

                paintManager.nextNode();
            }
        }
    }
}
