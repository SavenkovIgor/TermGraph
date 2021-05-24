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

#include "source/Helpers/appstyle.h"

void TermTree::setTreeNodeCoords(QPointF leftTopPoint)
{
    if (getAllNodesInTree().isEmpty()) {
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

PaintedTerm* TermTree::getNodeAtPoint(const QPointF& pt) const
{
    for (auto node : getAllNodesInTree()) {
        if (node->getNodeRect(CoordType::scene).contains(pt)) {
            return node;
        }
    }

    return nullptr;
}

RectGraphicItem& TermTree::rect() { return mRect; }

void TermTree::addTerm(PaintedTerm* term)
{
    int paintLayer = term->getPaintLevel();

    assert(paintLayer >= 0);
    if (paintLayer < 0)
        return;

    int increaseSizeCount = paintLayer - mStacks.size() + 1;
    for (int i = 0; i < increaseSizeCount; i++) {
        mStacks.push_back(NodeVerticalStack());
    }

    term->setParentItem(&mRect);
    mStacks[paintLayer].addTerm(term);
}

bool TermTree::hasTerm(PaintedTerm* term) const
{
    for (const auto& stack : mStacks) {
        if (stack.hasNode(term)) {
            return true;
        }
    }
    return false;
}

bool TermTree::hasEdge(EdgeOld* edge) const
{
    auto* rootTerm = static_cast<PaintedTerm*>(edge->getRoot());
    auto* leafTerm = static_cast<PaintedTerm*>(edge->getLeaf());
    return hasTerm(rootTerm) && hasTerm(leafTerm);
}

QRectF TermTree::getTreeRect(CoordType inCoordinates) const
{
    QRectF ret = QRectF(QPointF(), baseSize());

    switch (inCoordinates) {
    case CoordType::zeroPoint: break;
    case CoordType::local: ret = ret.translated(mRect.pos()); break;
    case CoordType::scene: ret = ret.translated(mRect.scenePos()); break;
    }

    return ret;
}

QSizeF TermTree::baseSize() const
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

double TermTree::square() const
{
    auto size = baseSize();
    return size.width() * size.height();
}

PaintedTerm::List TermTree::getAllNodesInTree() const
{
    PaintedTerm::List ret;

    for (const auto& stack : mStacks)
        ret << stack.nodes();

    return ret;
}

qreal TermTree::getMaxStackHeight() const
{
    qreal maxHeight = 0.0;

    for (const auto& stack : mStacks)
        maxHeight = std::max(maxHeight, stack.size().height());

    return maxHeight;
}
