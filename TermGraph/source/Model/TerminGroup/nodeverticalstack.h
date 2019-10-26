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

#pragma once

#include <QList>

#include "source/Model/Termin/paintedterm.h"
#include "source/Model/TerminEdge/edge.h"

class NodeVerticalStackTools {
public:
    static QSizeF getNodeVerticalStackedSize(const PaintedTerm::List &nodes);
};

class NodeVerticalStack
{
public:
    using List = QList<NodeVerticalStack*>;

    NodeVerticalStack();
    ~NodeVerticalStack();

    void addTerm(PaintedTerm* term);
    bool hasTerm(PaintedTerm* term) const;

    PaintedTerm::List getAllNodesInStack() const;

    QSizeF getSize() const;  // Clearly counted value. Ignoring real node positions

    void sortTerms();
    void placeTerms(QPointF centerPoint);
    void setNeighbours();

    // Animation
    QParallelAnimationGroup animationGroup;
    QPropertyAnimation swAnim1;
    QPropertyAnimation swAnim2;

    void swapNodes(PaintedTerm *n1, PaintedTerm *n2)
    {
        if (animationGroup.state() != QAbstractAnimation::Stopped) {
            return;  // Very important instruction.
        }
        // Because of lags, timer can still work, but animation will slow down and it cause
        // animation lags

        if (!hasTerm(n1) || !hasTerm(n2)) {
            return;
        }

        //currAnimLevel = n1->getPaintLayer();

        QRectF rc1 = n1->getNodeRect(CoordType::local);
        QRectF rc2 = n2->getNodeRect(CoordType::local);

        QPointF pos1, pos2;

        if (rc1.top() < rc2.top()) {
            pos1 = QPointF(n1->pos().x() + rc1.width()/2 - rc2.width()/2, n1->pos().y());
            pos2 = QPointF(n2->pos().x() + rc2.width()/2 - rc1.width()/2, n2->pos().y() - rc1.height() + rc2.height());
        } else {
            pos1 = QPointF(n1->pos().x() + rc1.width()/2-rc2.width()/2, n1->pos().y() - rc2.height() + rc1.height());
            pos2 = QPointF(n2->pos().x() + rc2.width()/2-rc1.width()/2, n2->pos().y());
        }

        swAnim1.setTargetObject(n1);
        swAnim1.setStartValue(n1->pos());
        swAnim1.setEndValue(pos2);

        swAnim2.setTargetObject(n2);
        swAnim2.setStartValue(n2->pos());
        swAnim2.setEndValue(pos1);

        animationGroup.start();
    }

private:
    PaintedTerm::List terms;
};
