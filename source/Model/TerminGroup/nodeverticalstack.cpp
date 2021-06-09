/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
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

#include "source/Model/TerminGroup/nodeverticalstack.h"

#include "source/Helpers/helpstuff.h"

QSizeF NodeVerticalStackTools::getNodeVerticalStackedSize(const PaintedTerm::List& nodes)
{
    SizeList sizeList;

    for (auto node : nodes)
        sizeList.push_back(node->getFrameRect(CoordType::zeroPoint).size());

    return HelpStuff::getStackedSize(sizeList, Qt::Vertical);
}

void NodeVerticalStack::addTerm(PaintedTerm* term) { mTerms.push_back(term); }

bool NodeVerticalStack::hasNode(PaintedTerm* term) const
{
    for (auto stackTerm : mTerms) {
        if (stackTerm == term) {
            return true;
        }
    }
    return false;
}

PaintedTerm::List NodeVerticalStack::nodes() const { return mTerms; }

// Clearly counted value. Ignoring real node positions
QSizeF NodeVerticalStack::size() const { return NodeVerticalStackTools::getNodeVerticalStackedSize(mTerms); }

void NodeVerticalStack::placeTerms(QPointF centerPoint)
{
    PaintedTerm::List placingTerms = mTerms;

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

bool NodeVerticalStack::isRootStack() const
{
    if (!mTerms.empty())
        return mTerms.front()->isRoot();

    return false;
}

std::vector<NodeVerticalStack::NodePack> NodeVerticalStack::getNodePacks(const PaintedTerm::List& terms)
{
    std::vector<NodePack> ret;

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
            auto nodes = std::vector<PaintedTerm*>();
            nodes.push_back(term);
            ret.push_back(NodePack(optimalPt, nodes));
        }
    }

    return ret;
}

void NodeVerticalStack::sortNodePacks(std::vector<NodeVerticalStack::NodePack>& pack)
{
    auto order = [](const NodeVerticalStack::NodePack& s1, const NodeVerticalStack::NodePack& s2) {
        return s1.first.y() < s2.first.y();
    };
    std::sort(pack.begin(), pack.end(), order);

    auto innerOrder = [](const PaintedTerm* t1, const PaintedTerm* t2) {
        return t1->decoratedTerm() < t2->decoratedTerm();
    };

    for ([[maybe_unused]] auto& [pt, nodes] : pack) {
        std::sort(nodes.begin(), nodes.end(), innerOrder);
    }
}

PaintedTerm::List NodeVerticalStack::flatNodePack(const std::vector<NodeVerticalStack::NodePack>& pack)
{
    PaintedTerm::List ret;

    for ([[maybe_unused]] const auto& [pt, nodes] : pack) {
        for (auto* node : nodes)
            ret.push_back(node);
    }

    return ret;
}
