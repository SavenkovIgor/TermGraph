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

#include "source/Model/TerminGroup/termtree.h"

#include <ranges>

#include "source/Helpers/appstyle.h"

PaintedForest::PaintedForest(const GraphData<PaintedTerm, PEdge>& data)
    : Forest<PaintedTerm, PEdge>(data)
{
    for (auto term : data.nodes)
        term->setParentItem(&mRect);

    auto layersCount = 0;
    for (auto node : data.nodes)
        layersCount = std::max(layersCount, level(node));

    for (int i = 0; i <= layersCount; i++)
        mStacks.push_back(NodeVerticalStack());

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
    PaintedTerm::List parentsList;

    rootsVisiter(term, [&parentsList](auto node) {
        if (std::find(parentsList.begin(), parentsList.end(), node) == parentsList.end()) {
            parentsList.push_back(node);
        }
        return false;
    });

    if (parentsList.empty())
        return "";

    // Sorting parents list
    std::ranges::sort(parentsList, [this](auto n1, auto n2) { return level(n1) > level(n2); });

    QStringList definitions;

    for (auto node : parentsList)
        definitions << node->cache().termAndDefinition(true);

    // Add this definition
    definitions << term->cache().termAndDefinition(true);

    return definitions.join("<br><br>");
}

RectGraphicItem& PaintedForest::rect() { return mRect; }

bool PaintedForest::hasTerm(PaintedTerm* term) const
{
    for (const auto& stack : mStacks) {
        if (stack.hasNode(term)) {
            return true;
        }
    }
    return false;
}

bool PaintedForest::hasEdge(PaintedEdge::Ptr edge) const
{
    auto* rootTerm = static_cast<PaintedTerm*>(edge->root().get());
    auto* leafTerm = static_cast<PaintedTerm*>(edge->leaf().get());
    return hasTerm(rootTerm) && hasTerm(leafTerm);
}

QRectF PaintedForest::getTreeRect(CoordType inCoordinates) const
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
