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

#include "nodeverticalstack.h"

#include "source/Helpers/helpstuff.h"

QSizeF NodeVerticalStackTools::getNodeVerticalStackedSize(const PaintedTerm::List& nodes)
{
    SizesList sizeList;

    for (auto node : nodes)
        sizeList.push_back(node->getFrameRect(CoordType::zeroPoint).size());

    return HelpStuff::getStackedSize(sizeList, Qt::Vertical);
}

NodeVerticalStack::NodeVerticalStack()
{
    animationGroup.addAnimation(&swAnim1);
    animationGroup.addAnimation(&swAnim2);

    swAnim1.setPropertyName("pos");
    swAnim2.setPropertyName("pos");

    swAnim1.setEasingCurve(QEasingCurve::InOutQuad);
    swAnim2.setEasingCurve(QEasingCurve::InOutQuad);
}

NodeVerticalStack::~NodeVerticalStack()
{
    terms.clear();
}

void NodeVerticalStack::addTerm(PaintedTerm* term)
{
    terms << term;
}

bool NodeVerticalStack::hasTerm(PaintedTerm* term) const
{
    for (auto stackTerm : terms) {
        if (stackTerm == term) {
            return true;
        }
    }
    return false;
}

PaintedTerm::List NodeVerticalStack::getAllNodesInStack() const
{
    return terms;
}

QSizeF NodeVerticalStack::getSize() const
{
    return NodeVerticalStackTools::getNodeVerticalStackedSize(terms);
}

void NodeVerticalStack::sortTerms()
{
    // Сначала сортируем по убыванию количества связей

    PaintedTerm::List tmpList = terms;
    int               nMax    = 0;

    for (int i = 0; i < tmpList.size(); i++) {
        nMax = i;
        for (int j = i + 1; j < tmpList.size(); j++) {
            if (tmpList[j]->getUpLevels() > tmpList[nMax]->getUpLevels()) {
                nMax = j;
            }
        }
        tmpList.swap(i, nMax);
    }

    PaintedTerm::List ret;
    // Формируем новый список так чтобы максимально нагруженные вершины были ближе к центру
    for (int i = 0; i < tmpList.size(); i++) {
        if (i % 2 == 0)
            ret.push_front(tmpList[i]);
        else
            ret.push_back(tmpList[i]);
    }

    terms = ret;
}

void NodeVerticalStack::placeTerms(QPointF centerPoint)
{
    //sortTerms(); //TODO: Check. maybe causing random bug

    auto    stackSize = getSize();
    QPointF startPoint(centerPoint.x(), centerPoint.y() - stackSize.height() / 2);

    auto placingPoint = startPoint;

    for (auto term : terms) {
        auto frameSize = term->getFrameRect(CoordType::zeroPoint).size();
        auto rectSize  = term->getNodeRect(CoordType::zeroPoint).size();
        placingPoint.ry() += frameSize.height() / 2;

        QPointF leftTopPt;
        leftTopPt.setX(placingPoint.x() - rectSize.width() / 2);
        leftTopPt.setY(placingPoint.y() - rectSize.height() / 2);

        term->setPos(leftTopPt);

        placingPoint.ry() += frameSize.height() / 2;
    }
}

void NodeVerticalStack::setNeighbours()
{
    // Casting list to GraphicItemTerm
    GraphTerm::List castedList;

    for (auto term : terms) {
        castedList << static_cast<GraphTerm*>(term);
    }

    for (auto term : terms) {
        term->clearNeighboursList();
        term->addLayerNeighbours(castedList);
    }
}
