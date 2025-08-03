// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

module;

#include <vector>

#include <QRectF>

export module PaintedForest;

import AppStyle;
import CommonTools.HandyTypes;
import Enums.CoordType;
import Graph;
import NodeVerticalStack;
import PaintedEdge;
import PaintedTerm;
import RectGraphicItem;
import TermGroup.TermPositioner;

using namespace std;
namespace rng = std::ranges;

export class PaintedForest : public graph::Forest<PaintedTerm, PaintedEdge>, public TermPositioner
{
public:
    // Constructor/Destructor
    PaintedForest(const GraphData<PaintedTerm, PaintedEdge>& data)
        : Forest<PaintedTerm, PaintedEdge>(data)
    {
        auto asListSize = [](auto num) { return static_cast<vector<NodeVerticalStack>::size_type>(num); };

        for (const auto& term : data.nodes) {
            term->setParentItem(&mRect);
        }

        for (const auto& edge : data.edges) {
            edge->setParentItem(&mRect);
        }

        auto layersCount = 0;
        for (const auto& node : data.nodes) {
            layersCount = max(layersCount, level(node));
        }

        for (int i = 0; i <= layersCount; i++) {
            mStacks.push_back(NodeVerticalStack(this));
        }

        for (const auto& term : data.nodes) {
            int paintLevel = level(term);

            if (0 <= paintLevel && paintLevel < asInt(mStacks.size())) {
                mStacks[asListSize(paintLevel)].addTerm(term);
            }
        }
    }
    ~PaintedForest() override = default;

    RectGraphicItem& rect() { return mRect; }

    // TreeInfo
    bool hasTerm(PaintedTerm::Ptr term) const
    {
        for (const auto& stack : mStacks) {
            if (stack.hasNode(term)) {
                return true;
            }
        }
        return false;
    }

    QRectF frameRect(CoordType inCoordinates) const
    {
        QRectF ret = QRectF(QPointF(), baseSize());

        switch (inCoordinates) {
        case CoordType::zeroPoint: break;
        case CoordType::local: ret = ret.translated(mRect.pos()); break;
        case CoordType::scene: ret = ret.translated(mRect.scenePos()); break;
        }

        return ret;
    }

    // Clearly counted value. Ignoring real node positions
    QSizeF baseSize() const
    {
        qreal width  = 0.0;
        qreal height = 0.0;

        for (const auto& stack : mStacks) {
            auto size = stack.size();
            width += size.width();
            height = max(height, size.height());
        }

        if (!mStacks.empty()) {
            width += (asInt(mStacks.size()) - 1) * AppStyle::Sizes::treeLayerHorizontalSpacer;
        }

        return {width, height};
    }

    double square() const
    {
        auto size = baseSize();
        return size.width() * size.height();
    }

    // Implementation of TermPositioner
    optional<QPointF> preferredPositionFor(PaintedTerm::Ptr term) const override
    {
        return optimalRootsBasedPosition(term);
    }

    // Internal counts and preparations
    void setTreeNodeCoords(QPointF leftTopPoint = QPointF())
    {
        if (getAllNodesInTree().empty()) {
            return;
        }

        qreal allTreeHeight = getMaxStackHeight();

        auto startPoint = leftTopPoint;
        startPoint.ry() += allTreeHeight / 2;

        auto centerPoint = startPoint;

        for (auto& stack : mStacks) {
            auto stackSize = stack.size();
            centerPoint.rx() += stackSize.width() / 2;
            stack.placeTerms(centerPoint);
            centerPoint.rx() += stackSize.width() / 2 + AppStyle::Sizes::treeLayerHorizontalSpacer;
        }
    }

    optional<QPointF> optimalRootsBasedPosition(const PaintedTerm::Ptr term) const
    {
        auto rNodes = rootNodes(term);

        if (rNodes.empty()) {
            return nullopt;
        }

        double sumOfYCoords = 0.0;

        for (const auto& node : rNodes) {
            sumOfYCoords += node->getCenter(CoordType::scene).y();
        }

        auto averageY = sumOfYCoords / asInt(rNodes.size());

        auto centerPoint = term->getCenter(CoordType::scene);
        centerPoint.setY(averageY);

        return centerPoint;
    }

    PaintedTerm::OptPtr getNodeAtPoint(const QPointF& pt) const
    {
        for (auto node : getAllNodesInTree()) {
            if (node->getNodeRect(CoordType::scene).contains(pt)) {
                return node;
            }
        }

        return nullopt;
    }

    QString getHierarchyDefinition(PaintedTerm::Ptr term)
    {
        PaintedTerm::List parentsList;

        rootsVisiter(term, [&parentsList](auto node) {
            if (rng::find(parentsList, node) == parentsList.end()) {
                parentsList.push_back(node);
            }
            return false;
        });

        if (parentsList.empty()) {
            return "";
        }

        // Sorting parents list
        rng::sort(parentsList, [this](auto n1, auto n2) { return level(n1) > level(n2); });

        QStringList definitions;

        for (const auto& node : parentsList) {
            definitions << node->cache().termAndDefinition(true);
        }

        // Add this definition
        definitions << term->cache().termAndDefinition(true);

        return definitions.join("<br><br>");
    }

    void selectTerm(const PaintedTerm::Ptr& term, bool selected)
    {
        assert(hasTerm(term));

        term->setSelection(selected);

        rootsVisiter(
            term,
            [this, selected](auto node) {
                node->setRelativeSelection(selected);

                for (auto edge : edgesToRoots().at(node)) {
                    edge->setSelectedBackward(selected);
                }

                return false;
            },
            true);

        leafsVisiter(
            term,
            [this, selected](auto node) {
                node->setRelativeSelection(selected);

                for (auto edge : edgesToLeafs().at(node)) {
                    edge->setSelectedForward(selected);
                }

                return false;
            },
            true);
    }

    // Deletions
    PaintedForest(const PaintedForest&) = delete;

private:
    // Internal Info
    PaintedTerm::List getAllNodesInTree() const
    {
        PaintedTerm::List ret;

        for (const auto& stack : mStacks) {
            for (const auto& node : stack.nodes()) {
                ret.push_back(node);
            }
        }

        return ret;
    }

    qreal getMaxStackHeight() const
    {
        qreal maxHeight = 0.0;

        for (const auto& stack : mStacks) {
            maxHeight = max(maxHeight, stack.size().height());
        }

        return maxHeight;
    }

private: // Members
    vector<NodeVerticalStack> mStacks;
    RectGraphicItem           mRect;
};
