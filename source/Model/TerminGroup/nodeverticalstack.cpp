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

    PaintedTerm::List placingTerms = this->terms;

    if (!isRootStack()) {
        auto packs = getNodePacks(placingTerms);
        sortNodePacks(packs);
        placingTerms = flatNodePack(packs);
    }

    auto    stackSize = NodeVerticalStackTools::getNodeVerticalStackedSize(placingTerms);
    QPointF startPoint(centerPoint.x(), centerPoint.y() - stackSize.height() / 2);

    auto placingPoint = startPoint;

    for (auto term : placingTerms) {
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

bool NodeVerticalStack::isRootStack()
{
    if (!terms.isEmpty())
        return terms.first()->isRoot();

    return false;
}

QList<NodeVerticalStack::NodePack> NodeVerticalStack::getNodePacks(const PaintedTerm::List& terms)
{
    QList<NodePack> ret;

    for (auto* term : terms) {
        auto rootsPositionOpt = term->optimalRootsBasedPosition();
        auto optimalPt        = rootsPositionOpt.value_or(term->getCenter(CoordType::scene));

        // Selecting pack for insert
        bool inserted = false;

        for (auto& [point, nodes] : ret) {
            bool equalX = std::abs(point.x() - optimalPt.x()) < 0.1;
            bool equalY = std::abs(point.y() - optimalPt.y()) < 0.1;
            if (equalX && equalY) {
                nodes.push_back(term);
                inserted = true;
                break;
            }
        }

        if (!inserted) {
            auto nodes = QList<PaintedTerm*>();
            nodes.push_back(term);
            ret.push_back(NodePack(optimalPt, nodes));
        }
    }

    return ret;
}

void NodeVerticalStack::sortNodePacks(QList<NodeVerticalStack::NodePack>& pack)
{
    auto order = [](const NodeVerticalStack::NodePack& s1, const NodeVerticalStack::NodePack& s2) {
        return s1.first.y() < s2.first.y();
    };
    std::sort(pack.begin(), pack.end(), order);

    auto innerOrder = [](const PaintedTerm* t1, const PaintedTerm* t2) {
        return t1->getSmallName() < t2->getSmallName();
    };

    for ([[maybe_unused]] auto& [pt, nodes] : pack) {
        std::sort(nodes.begin(), nodes.end(), innerOrder);
    }
}

PaintedTerm::List NodeVerticalStack::flatNodePack(const QList<NodeVerticalStack::NodePack>& pack)
{
    PaintedTerm::List ret;

    for ([[maybe_unused]] const auto& [pt, nodes] : pack) {
        for (auto* node : nodes)
            ret.push_back(node);
    }

    return ret;
}
