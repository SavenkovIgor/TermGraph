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

#include "paintqueuemanager.h"

void PaintManager::addClearRect(const QRectF &rect, bool paintNow)
{
    clearPaintArea.enqueue(rect);

    if (paintNow) {
        emit paintGroupQueue();
    }
}

QRectF PaintManager::currentClearRect()
{
    return clearPaintArea.head();
}

void PaintManager::nextClearRect()
{
    clearPaintArea.dequeue();
}

bool PaintManager::clearQueueEmpty() const
{
    return clearPaintArea.isEmpty();
}

void PaintManager::clearAllQueues()
{
    groupRectsForPaint.clear();
    groupNamesForPaint.clear();
    edgesForPaint.clear();
}

void PaintManager::addGroup(TermGroup *group)
{
    groupRectsForPaint.enqueue(QPair<QRectF, QColor>(group->getGroupRect(), group->getGroupFillColor()));
    groupNamesForPaint.enqueue(QPair<QPointF, QString>(group->getNamePos(), group->getName()));

    for (auto edge : group->getAllEdgesForPainting())
        edgesForPaint.enqueue(edge);

    emit paintGroupQueue();
}

void PaintManager::addRect(const QRectF &rect, const QColor &color)
{
    groupRectsForPaint.enqueue(QPair<QRectF, QColor>(rect, color));
}

void PaintManager::nextGroupRect()
{
    groupRectsForPaint.dequeue();
}

bool PaintManager::groupRectQueueEmpty() const
{
    return groupRectsForPaint.isEmpty();
}

QRectF PaintManager::currentGroupRect() const
{
    return groupRectsForPaint.head().first;
}

QColor PaintManager::currentGroupFillColor() const
{
    return groupRectsForPaint.head().second;
}

void PaintManager::nextGroupName()
{
    groupNamesForPaint.dequeue();
}

bool PaintManager::groupNamesQueueEmpty() const
{
    return groupNamesForPaint.isEmpty();
}

QString PaintManager::currentGroupName() const
{
    return groupNamesForPaint.head().second;
}

QPointF PaintManager::currentGroupNamePos() const
{
    return groupNamesForPaint.head().first;
}

void PaintManager::nextEdge()
{
    edgesForPaint.dequeue();
}

bool PaintManager::edgeQueueEmpty() const
{
    return edgesForPaint.isEmpty();
}

QColor PaintManager::getEdgeColor() const
{
    return edgesForPaint.head()->getEdgeColor();
}

QPointF PaintManager::currentFirstEdgePoint() const
{
    auto graphTerm = edgesForPaint.head()->getRoot();
    auto paintedTerm = dynamic_cast<PaintedTerm*>(graphTerm);
    auto pt = paintedTerm->scenePos();
    pt += paintedTerm->getNodeRect(CoordType::zeroPoint).center();
    return pt;
}

QPointF PaintManager::currentLastEdgePoint() const
{
    auto graphTerm = edgesForPaint.head()->getLeaf();
    auto paintedTerm = dynamic_cast<PaintedTerm*>(graphTerm);
    auto pt = paintedTerm->scenePos();
    pt += paintedTerm->getNodeRect(CoordType::zeroPoint).center();
    return pt;
}

void PaintManager::setPaintInProcessFlag(bool paintNow)
{
    paintInProcessFlag = paintNow;
}

bool PaintManager::isPaintInProcessNow() const
{
    return paintInProcessFlag;
}

void PaintManager::startCheckTimer()
{
    paintSpeedTimer.start();
}

void PaintManager::restartCheckTimer(const QString& label)
{
    qDebug() << label << " " << paintSpeedTimer.restart();
}
