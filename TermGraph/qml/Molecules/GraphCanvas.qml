import QtQuick 2.13
import QtQuick.Controls 2.13

import "../Js/NodePaint.js" as JsPaint

Control {

    height: scene.sceneRect.height
    width: scene.sceneRect.width

    background: Rectangle { color: scene.getSceneBackgroundColor() }

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
            // console.log("Paint call")
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
