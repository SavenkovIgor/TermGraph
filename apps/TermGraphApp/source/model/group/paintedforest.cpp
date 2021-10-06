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

#include "source/model/group/paintedforest.h"

#include "source/helpers/appstyle.h"

PaintedForest::PaintedForest(const GraphData<PaintedTerm, PaintedEdge>& data)
    : Forest<PaintedTerm, PaintedEdge>(data)
{
    for (auto term : data.nodes)
        term->setParentItem(&mRect);

    for (auto edge : data.edges)
        edge->setParentItem(&mRect);

    auto layersCount = 0;
    for (auto node : data.nodes)
        layersCount = std::max(layersCount, level(node));

    for (int i = 0; i <= layersCount; i++)
        mStacks.push_back(NodeVerticalStack(this));

    for (auto term : data.nodes) {
        int paintLevel = level(term);

        if (0 <= paintLevel && paintLevel < mStacks.size())
            mStacks[paintLevel].addTerm(term);
    }
}

void PaintedForest::setTreeNodeCoords(QPointF leftTopPoint)
{
    if (getAllNodesInTree().empty())
        return;

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

opt<QPointF> PaintedForest::optimalRootsBasedPosition(const PaintedTerm::Ptr term) const
{
    auto rNodes = rootNodes(term);

    if (rNodes.empty())
        return std::nullopt;

    double sumOfYCoords = 0.0;

    for (auto node : rNodes)
        sumOfYCoords += node->getCenter(CoordType::scene).y();

    auto averageY = sumOfYCoords / rNodes.size();

    auto centerPoint = term->getCenter(CoordType::scene);
    centerPoint.setY(averageY);

    return centerPoint;
}

PaintedTerm::OptPtr PaintedForest::getNodeAtPoint(const QPointF& pt) const
{
    for (auto node : getAllNodesInTree()) {
        if (node->getNodeRect(CoordType::scene).contains(pt)) {
            return node;
        }
    }

    return std::nullopt;
}

QString PaintedForest::getHierarchyDefinition(PaintedTerm::Ptr term)
{
    using namespace std;

    PaintedTerm::List parentsList;

    rootsVisiter(term, [&parentsList](auto node) {
        if (find(begin(parentsList), end(parentsList), node) == parentsList.end()) {
            parentsList.push_back(node);
        }
        return false;
    });

    if (parentsList.empty())
        return "";

    // Sorting parents list
    sort(begin(parentsList), end(parentsList), [this](auto n1, auto n2) { return level(n1) > level(n2); });

    QStringList definitions;

    for (auto node : parentsList)
        definitions << node->cache().termAndDefinition(true);

    // Add this definition
    definitions << term->cache().termAndDefinition(true);

    return definitions.join("<br><br>");
}

void PaintedForest::selectTerm(const PaintedTerm::Ptr& term, bool selected)
{
    assert(hasTerm(term));

    term->setSelection(selected);

    rootsVisiter(
        term,
        [this, selected](auto node) {
            node->setRelativeSelection(selected);

            for (auto edge : edgesToRoots().at(node))
                edge->setSelectedBackward(selected);

            return false;
        },
        true);

    leafsVisiter(
        term,
        [this, selected](auto node) {
            node->setRelativeSelection(selected);

            for (auto edge : edgesToLeafs().at(node))
                edge->setSelectedForward(selected);

            return false;
        },
        true);
}

RectGraphicItem& PaintedForest::rect() { return mRect; }

bool PaintedForest::hasTerm(PaintedTerm::Ptr term) const
{
    for (const auto& stack : mStacks) {
        if (stack.hasNode(term)) {
            return true;
        }
    }
    return false;
}

QRectF PaintedForest::frameRect(CoordType inCoordinates) const
{
    QRectF ret = QRectF(QPointF(), baseSize());

    switch (inCoordinates) {
    case CoordType::zeroPoint: break;
    case CoordType::local: ret = ret.translated(mRect.pos()); break;
    case CoordType::scene: ret = ret.translated(mRect.scenePos()); break;
    }

    return ret;
}

QSizeF PaintedForest::baseSize() const
{
    qreal width  = 0.0;
    qreal height = 0.0;

    for (const auto& stack : mStacks) {
        auto size = stack.size();
        width += size.width();
        height = std::max(height, size.height());
    }

    if (!mStacks.empty()) {
        width += (mStacks.size() - 1) * AppStyle::Sizes::treeLayerHorizontalSpacer;
    }

    return QSizeF(width, height);
}

double PaintedForest::square() const
{
    auto size = baseSize();
    return size.width() * size.height();
}

PaintedTerm::List PaintedForest::getAllNodesInTree() const
{
    PaintedTerm::List ret;

    for (const auto& stack : mStacks)
        for (auto node : stack.nodes())
            ret.push_back(node);

    return ret;
}

qreal PaintedForest::getMaxStackHeight() const
{
    qreal maxHeight = 0.0;

    for (const auto& stack : mStacks)
        maxHeight = std::max(maxHeight, stack.size().height());

    return maxHeight;
}