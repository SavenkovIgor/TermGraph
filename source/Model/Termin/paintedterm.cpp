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

#include "source/Model/Termin/paintedterm.h"

#include "source/Helpers/appstyle.h"
#include "source/Model/TerminEdge/edge.h"

bool PaintedTerm::someoneHover  = false;
bool PaintedTerm::someoneSelect = false;

QList<Qt::Edge> PaintedTerm::sides;

PaintedTerm::PaintedTerm(const NodeInfoContainer& info, QObject* parent)
    : QObject(parent)
    , GraphTerm(info)
    , GraphicItem()
{
    if (sides.isEmpty()) {
        sides << Qt::BottomEdge;
        sides << Qt::TopEdge;
        sides << Qt::RightEdge;
        sides << Qt::LeftEdge;
    }
    adjustRectSizeForName();
}

QRectF PaintedTerm::rect() const
{
    return getNodeRect(CoordType::scene);
}

int PaintedTerm::getUpLevels([[maybe_unused]] int pLevel)  // TODO: check why plevel unused
{
    int ret = -1;
    for (GraphTerm* t : getLeafNodes()) {
        PaintedTerm* n = static_cast<PaintedTerm*>(t);
        ret            = std::max(ret, n->getUpLevels());
    }

    if (ret == -1)
        ret = getPaintLevel();

    return ret;
}

void PaintedTerm::setRelatedPaintUp(bool val)
{
    for (auto* edge : getEdgesToLeafs())
        dynamic_cast<Edge*>(edge)->setSelectedForward(val);

    for (auto node : getLeafNodes()) {
        auto* paintNode = static_cast<PaintedTerm*>(node);
        if (paintNode->mRelativePaint != val) {
            paintNode->mRelativePaint = val;
            emit paintNode->selectionChanged();
            paintNode->setRelatedPaintUp(val);
        }
    }
}

void PaintedTerm::setRelatedPaintDown(bool val)
{
    for (auto* edge : getEdgesToRoots())
        dynamic_cast<Edge*>(edge)->setSelectedBackward(val);

    for (auto* node : getRootNodes()) {
        auto* paintNode = static_cast<PaintedTerm*>(node);
        if (paintNode->mRelativePaint != val) {
            paintNode->mRelativePaint = val;
            emit paintNode->selectionChanged();
            paintNode->setRelatedPaintDown(val);
        }
    }
}

QLineF PaintedTerm::getRectLine(Qt::Edge side)
{
    QRectF rc = getNodeRect(CoordType::local);

    switch (side) {
    case Qt::TopEdge:
        return QLineF(rc.topLeft(), rc.topRight());

    case Qt::RightEdge:
        return QLineF(rc.topRight(), rc.bottomRight());

    case Qt::BottomEdge:
        return QLineF(rc.bottomLeft(), rc.bottomRight());

    case Qt::LeftEdge:
        return QLineF(rc.topLeft(), rc.bottomLeft());
    }
    return QLineF();
}

QRectF PaintedTerm::getNodeRect(CoordType inCoordinates) const
{
    switch (inCoordinates) {
    case CoordType::zeroPoint:
        return QRectF(QPointF(), nodeSize);
    case CoordType::local:
        return QRectF(pos(), nodeSize);
    case CoordType::scene:
        return QRectF(scenePos(), nodeSize);
    }

    assert(false);  // must be unreachable
    return QRectF();
}

QRectF PaintedTerm::getFrameRect(CoordType inCoordinates) const
{
    qreal  val = qBound(0.0, nodeSize.width() * 0.2, 8.0);
    QRectF ret = getNodeRect(inCoordinates);
    return addMarginsToRect(ret, val);
}

QPointF PaintedTerm::getCenter(CoordType inCoordinates) const
{
    return getNodeRect(inCoordinates).center();
}

QRectF PaintedTerm::addMarginsToRect(QRectF rc, qreal mrg)
{
    QMarginsF mrgObj(mrg, mrg, mrg, mrg);
    return rc.marginsAdded(mrgObj);
}

void PaintedTerm::updateCornerRadius()
{
    mCornerRadius = nodeSize.height() * 0.2;
}

QColor PaintedTerm::color() const
{
    return baseColor(getNodeType(), isSelectedAnyway());
}

void PaintedTerm::adjustRectSizeForName()
{
    QSizeF nameSize = additionalInfo().decoratedTermSize();
    nodeSize.setWidth(nameSize.width() + 16);
    nodeSize.setHeight(nameSize.height() + 4);
    updateCornerRadius();
}

QColor PaintedTerm::baseColor(NodeType type, bool selected)
{
    switch (type) {
    case NodeType::orphan:
        return selected ? AppStyle::Colors::Nodes::orphanSelected : AppStyle::Colors::Nodes::orphan;
    case NodeType::root:
        return selected ? AppStyle::Colors::Nodes::rootSelected : AppStyle::Colors::Nodes::root;
    case NodeType::endLeaf:
        return selected ? AppStyle::Colors::Nodes::leafSelected : AppStyle::Colors::Nodes::leaf;
    case NodeType::middleLeaf:
        return selected ? AppStyle::Colors::Nodes::leafSelected : AppStyle::Colors::Nodes::leaf;
    }

    assert(false);  // must be unreachable
    return AppStyle::Colors::Nodes::orphan;
}

qreal PaintedTerm::cornerRadius() const
{
    return mCornerRadius;
}

void PaintedTerm::setSelection(const bool& selected)
{
    if (mThisSelected != selected) {
        mThisSelected = selected;

        someoneSelect = selected;
        mRelativePaint = selected;

        setRelatedPaintDown(selected);
        setRelatedPaintUp(selected);

        emit selectionChanged();
    }
}

bool PaintedTerm::isSelectedAnyway() const
{
    return mThisSelected || mRelativePaint;
}

bool PaintedTerm::isThisSelected() const
{
    return mThisSelected;
}

opt<QPointF> PaintedTerm::optimalRootsBasedPosition() const
{
    auto edges = getEdgesToRoots();

    if (edges.isEmpty())
        return std::nullopt;

    double sumOfYCoords = 0.0;

    for (auto* edge : edges) {
        auto* parentNode = static_cast<PaintedTerm*>(edge->getRoot());
        sumOfYCoords += parentNode->getCenter(CoordType::scene).y();
    }

    auto averageY = sumOfYCoords / edges.size();

    auto centerPoint = getCenter(CoordType::scene);
    centerPoint.setY(averageY);

    return centerPoint;
}

QString PaintedTerm::decoratedTerm() const
{
    return additionalInfo().decoratedTerm();
}
