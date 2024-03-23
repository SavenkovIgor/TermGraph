// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

#include "source/model/group/nodeverticalstack.h"

#include <ranges>

namespace rng = std::ranges;

QSizeF NodeVerticalStackTools::getNodeVerticalStackedSize(const PaintedTerm::List& nodes)
{
    SizeList sizeList;

    for (const auto& node : nodes) {
        sizeList.push_back(node->getFrameRect(CoordType::zeroPoint).size());
    }

    return sizeList.totalStackedSize(Qt::Vertical);
}

NodeVerticalStack::NodeVerticalStack(PaintedForest* parentForest)
    : mParentForest(parentForest)
{}

void NodeVerticalStack::addTerm(PaintedTerm::Ptr term) { mTerms.push_back(term); }

bool NodeVerticalStack::hasNode(PaintedTerm::Ptr term) const { return rng::find(mTerms, term) != mTerms.end(); }

PaintedTerm::List NodeVerticalStack::nodes() const { return mTerms; }

// Clearly counted value. Ignoring real node positions
QSizeF NodeVerticalStack::size() const { return NodeVerticalStackTools::getNodeVerticalStackedSize(mTerms); }

void NodeVerticalStack::placeTerms(QPointF centerPoint)
{
    PaintedTerm::List placingTerms = mTerms;

    auto packs = getNodePacks(placingTerms, mParentForest);
    sortNodePacks(packs);
    placingTerms = flatNodePack(packs);

    auto    stackSize = NodeVerticalStackTools::getNodeVerticalStackedSize(placingTerms);
    QPointF startPoint(centerPoint.x(), centerPoint.y() - stackSize.height() / 2);

    auto placingPoint = startPoint;

    for (const auto& term : placingTerms) {
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

std::vector<NodeVerticalStack::NodePack> NodeVerticalStack::getNodePacks(const PaintedTerm::List& terms,
                                                                         const PaintedForest*     forest)
{
    std::vector<NodePack> ret;

    for (const auto& term : terms) {
        auto rootsPositionOpt = forest->optimalRootsBasedPosition(term);
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
            auto nodes = PaintedTerm::List();
            nodes.push_back(term);
            ret.push_back(NodePack(optimalPt, nodes));
        }
    }

    return ret;
}

void NodeVerticalStack::sortNodePacks(std::vector<NodeVerticalStack::NodePack>& pack)
{
    rng::sort(pack, [](const auto& s1, const auto& s2) { return s1.first.y() < s2.first.y(); });

    for (auto& [_, nodes] : pack) {
        rng::sort(nodes, [](const auto t1, const auto t2) { return t1->term() < t2->term(); });
    }
}

PaintedTerm::List NodeVerticalStack::flatNodePack(const std::vector<NodeVerticalStack::NodePack>& pack)
{
    PaintedTerm::List ret;

    for ([[maybe_unused]] const auto& [pt, nodes] : pack) {
        for (const auto& node : nodes) {
            ret.push_back(node);
        }
    }

    return ret;
}
