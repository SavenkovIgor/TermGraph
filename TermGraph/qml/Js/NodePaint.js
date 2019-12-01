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

.pragma library

function prepareRects(ctx)
{
    ctx.strokeStyle = "#000000";
    ctx.lineWidth = 2;
    ctx.textBaseline = "middle";
    ctx.font = "10pt sans-serif Arial";
}

function paintRoundedRect(ctx, rect, rectColor, radius)
{
    // Draw border
    ctx.beginPath();
    ctx.roundedRect(rect.x, rect.y, rect.width, rect.height, radius, radius);
    ctx.stroke();

    // Draw inner rect with the same path
    ctx.fillStyle = rectColor;
    ctx.fill();
}

function prepareEdge(ctx)
{
    ctx.lineWidth = 2;
}

function drawBLine(ctx, pt1, pt2, color)
{
    const coeff = 0.5;
    const width = pt2.x - pt1.x;
    const ptb1 = {x: pt1.x + width * coeff, y: pt1.y};
    const ptb2 = {x: pt2.x - width * coeff, y: pt2.y};

    ctx.moveTo(pt1.x, pt1.y);
    ctx.bezierCurveTo(ptb1.x, ptb1.y, ptb2.x, ptb2.y, pt2.x, pt2.y);
}

function paintGroupName(ctx, text, basePoint)
{
    ctx.textBaseline = "middle";
    ctx.font = "bold 10.5pt sans-serif Arial";

    ctx.fillStyle = "#FFFFFF";
    ctx.fillText(text, basePoint.x, basePoint.y);
}

function paintTextWithSplit(ctx, text, center, rect)
{
    ctx.fillStyle = "#FFFFFF";
    // Draw text
    const txtList = text.split('\n');

    const count = Object.keys(txtList).length;
    const firstLineWidth = ctx.measureText(txtList[0]).width;
    const posX = center.x - firstLineWidth / 2;

    if (count === 1) {
        ctx.fillText(txtList[0], posX, center.y);
    } else if (count === 2) {
        const heightQuater = rect.height / 4;

        ctx.fillText(txtList[0], posX, center.y - heightQuater);

        const secondLineWidth = ctx.measureText(txtList[1]).width;
        const pos2X = center.x - secondLineWidth / 2;
        ctx.fillText(txtList[1], pos2X, center.y + heightQuater);
    }
}

function paintRect(ctx, rect, borderColor)
{
    ctx.strokeStyle = borderColor;
    ctx.lineWidth = 1;
    ctx.beginPath();
    ctx.rect(rect.x, rect.y, rect.width, rect.height);
    ctx.stroke();
}

function clearRect(ctx, rect, offset)
{
    ctx.clearRect(rect.x - offset, rect.y - offset, rect.width + 2*offset, rect.height + 2*offset);
}

function paintFilledRect(ctx, rect, color)
{
    ctx.fillStyle = color;
    ctx.fillRect(rect.x, rect.y, rect.width, rect.height);
}
