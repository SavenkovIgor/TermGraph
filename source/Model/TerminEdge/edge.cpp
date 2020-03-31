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

#include "source/Model/TerminEdge/edge.h"

#include "source/Helpers/appstyle.h"
#include "source/Model/Termin/paintedterm.h"

Edge::Edge(QObject* parent)
    : QObject(parent)
    , GraphEdge()
    , GraphicItem()
{}

Edge::Edge(PaintedTerm* toRoot, PaintedTerm* toLeaf, EdgeType type, QObject* parent)
    : QObject(parent)
    , GraphEdge(toRoot, toLeaf)
    , GraphicItem()
{
    mType = type;
}

QColor Edge::getEdgeColor() const
{
    switch (mSelected) {
    case EdgeSelected::backward:
        return AppStyle::Colors::Edges::selected;
    case EdgeSelected::forward:
        return AppStyle::Colors::Edges::selectedAlt;
    default:
        break;
    }

    switch (mType) {
    case EdgeType::standart:
        return AppStyle::Colors::Edges::standard;
    case EdgeType::termin:
        return AppStyle::Colors::Edges::termin;
    case EdgeType::description:
        return AppStyle::Colors::Edges::description;
    case EdgeType::broken:
        return AppStyle::Colors::Edges::broken;
    }

    return AppStyle::Colors::Edges::standard;
}

void Edge::brokeEdge()
{
    getRoot()->removeEdgeToLeafs(this);
    getRoot()->addBrokenEdge(this);
    getLeaf()->removeEdgeToRoots(this);

    mType = EdgeType::broken;
}

void Edge::cutOutFromSides()
{
    getRoot()->removeEdgeToLeafs(this);
    getLeaf()->removeEdgeToRoots(this);
}

EdgeSelected Edge::selectedType() const
{
    return mSelected;
}

bool Edge::isSelected() const
{
    return mSelected == EdgeSelected::forward || mSelected == EdgeSelected::backward;
}

QRectF Edge::edgeRect() const
{
    QPointF pt1 = rootPoint();
    QPointF pt2 = leafPoint();

    QRectF rc = QRectF(pt1, pt2);
    rc        = rc.normalized();
    return rc;
}

int Edge::getLayerDistance()
{
    auto toRoot = dynamic_cast<PaintedTerm*>(getRoot());
    auto toLeaf = dynamic_cast<PaintedTerm*>(getLeaf());

    return std::abs(toRoot->getPaintLevel() - toLeaf->getPaintLevel());
}

qreal Edge::getXProjection()
{
    return edgeRect().width();
}

qreal Edge::getYProjection()
{
    return edgeRect().height();
}

QLineF Edge::getLine(bool swap)
{
    QPointF pt1 = rootPoint();
    QPointF pt2 = leafPoint();

    if (swap) {
        if (!swapPointRoot.isNull())
            pt1 = swapPointRoot;

        if (!swapPointLeaf.isNull())
            pt2 = swapPointLeaf;
    }

    return QLineF(pt1, pt2);
}

QPointF Edge::rootPoint() const
{
    auto paintedTerm = dynamic_cast<PaintedTerm*>(getRoot());
    return paintedTerm->getCenter(CoordType::scene);
}

QPointF Edge::leafPoint() const
{
    auto paintedTerm = dynamic_cast<PaintedTerm*>(getLeaf());
    return paintedTerm->getCenter(CoordType::scene);
}

Edge::List Edge::castToEdgeList(const GraphEdge::List& lst)
{
    Edge::List edgeLst;
    for (auto graphEdg : lst) {
        edgeLst << dynamic_cast<Edge*>(graphEdg);
    }
    return edgeLst;
}

void Edge::setSelectedForward(bool value)
{
    auto resultSelection = value ? EdgeSelected::forward : EdgeSelected::none;
    if (mSelected != resultSelection) {
        mSelected = resultSelection;
        emit selectionChanged();
    }
}

void Edge::setSelectedBackward(bool value)
{
    auto resultSelection = value ? EdgeSelected::backward : EdgeSelected::none;
    if (mSelected != resultSelection) {
        mSelected = resultSelection;
        emit selectionChanged();
    }
}
