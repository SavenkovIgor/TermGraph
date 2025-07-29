// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <ranges>
#include <vector>

#include <QPair>
#include <QPointF>
#include <QSizeF>

export module NodeVerticalStack;

import CommonTools.HandyTypes;
import Enums.CoordType;
import NodeVerticalStackTools;
import PaintedTerm;
import TermGroup.TermPositioner;

namespace rng = std::ranges;

export class NodeVerticalStack
{
public:
    using List     = std::vector<NodeVerticalStack>;
    using NodePack = QPair<QPointF, PaintedTerm::List>;

    NodeVerticalStack(TermPositioner* termPositioner)
        : mTermPositioner(termPositioner)
    {}

    ~NodeVerticalStack() = default;

    void addTerm(PaintedTerm::Ptr term) { mTerms.push_back(term); }

    void placeTerms(QPointF centerPoint)
    {
        PaintedTerm::List placingTerms = mTerms;

        auto packs = getNodePacks(placingTerms, mTermPositioner);
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

    // Clearly counted value. Ignoring real node positions
    QSizeF size() const { return NodeVerticalStackTools::getNodeVerticalStackedSize(mTerms); }

    bool hasNode(PaintedTerm::Ptr term) const { return rng::find(mTerms, term) != mTerms.end(); }

    PaintedTerm::List nodes() const { return mTerms; }

private:
    static std::vector<NodePack> getNodePacks(const PaintedTerm::List& terms, const TermPositioner* termPositioner)
    {
        std::vector<NodePack> ret;

        for (const auto& term : terms) {
            auto rootsPositionOpt = termPositioner->preferredPositionFor(term);
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

    static void sortNodePacks(std::vector<NodePack>& pack)
    {
        rng::sort(pack, [](const auto& s1, const auto& s2) { return s1.first.y() < s2.first.y(); });

        for (auto& [_, nodes] : pack) {
            rng::sort(nodes, [](const auto t1, const auto t2) { return t1->term() < t2->term(); });
        }
    }

    static PaintedTerm::List flatNodePack(const std::vector<NodePack>& pack)
    {
        PaintedTerm::List ret;

        for ([[maybe_unused]] const auto& [pt, nodes] : pack) {
            for (const auto& node : nodes) {
                ret.push_back(node);
            }
        }

        return ret;
    }

private: // Members
    PaintedTerm::List     mTerms;
    const TermPositioner* mTermPositioner = nullptr;
};
