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

void PaintManager::requestPaint()
{
    emit paintGroupQueue();
}

void PaintManager::clearAllQueues()
{
    edgesForPaint.clear();
}

void PaintManager::addGroup(TermGroup *group)
{
    for (auto edge : group->getAllEdgesForPainting())
        edgesForPaint.enqueue(edge);

    emit paintGroupQueue();
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
    return paintedTerm->getNodeRect(CoordType::scene).center();
}

QPointF PaintManager::currentLastEdgePoint() const
{
    auto graphTerm = edgesForPaint.head()->getLeaf();
    auto paintedTerm = dynamic_cast<PaintedTerm*>(graphTerm);
    return paintedTerm->getNodeRect(CoordType::scene).center();
}
