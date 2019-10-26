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

#include "edge.h"

QColor Edge::getEdgeColor() const
{
    switch (selected) {
    case EdgeSelected::backward: return AppStyle::Colors::Edges::selected;
    case EdgeSelected::forward:  return AppStyle::Colors::Edges::selectedAlt;
    default: break;
    }

    switch (type) {
    case EdgeType::standart:    return AppStyle::Colors::Edges::standard;
    case EdgeType::termin:      return AppStyle::Colors::Edges::termin;
    case EdgeType::description: return AppStyle::Colors::Edges::description;
    case EdgeType::broken:      return AppStyle::Colors::Edges::broken;
    }

    return AppStyle::Colors::Edges::standard;
}

void Edge::brokeEdge()
{
    getRoot()->removeEdgeToLeafs(this);
    getRoot()->addBrokenEdge(this);
    getLeaf()->removeEdgeToRoots(this);
    type = EdgeType::broken;
}

void Edge::cutOutFromSides()
{
    getRoot()->removeEdgeToLeafs(this);
    getLeaf()->removeEdgeToRoots(this);
}

Edge::Edge(PaintedTerm* from, PaintedTerm* to, EdgeType type) :
    GraphEdge(from, to)
{
    assert(from != to);
    assert(from != nullptr);
    assert(to != nullptr);
    this->type = type;
}

QRectF Edge::edgeRect() const
{
    QPointF pt1 = dynamic_cast<PaintedTerm*>(getRoot())->getCenter(CoordType::scene);
    QPointF pt2 = dynamic_cast<PaintedTerm*>(getLeaf())->getCenter(CoordType::scene);

    QRectF rc = QRectF(pt1, pt2);
    rc = rc.normalized();
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
    auto toRoot = dynamic_cast<PaintedTerm*>(getRoot());
    auto toLeaf = dynamic_cast<PaintedTerm*>(getLeaf());

    QPointF pt1 = toRoot->getCenter(CoordType::scene);
    QPointF pt2 = toLeaf->getCenter(CoordType::scene);

    if (swap) {
        if (!swapPointRoot.isNull())
            pt1 = swapPointRoot;

        if (!swapPointLeaf.isNull())
            pt2 = swapPointLeaf;
    }

    return QLineF( pt1, pt2 );
}

EdgesList Edge::castToEdgeList(GraphEdge::List lst)
{
    EdgesList edgeLst;
    for (auto graphEdg : lst) {
        edgeLst << dynamic_cast<Edge*>(graphEdg);
    }
    return  edgeLst;
}

void Edge::setSelectedForward(bool value)
{
    if (value) {
        selected = EdgeSelected::forward;
    } else {
        selected = EdgeSelected::none;
    }

    needPaint = true;
}

void Edge::setSelectedBackward(bool value)
{
    if (value) {
        selected = EdgeSelected::backward;
    } else {
        selected = EdgeSelected::none;
    }

    needPaint = true;
}
