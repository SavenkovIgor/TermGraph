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

#include "source/Model/TerminEdge/edge.h"

#include "source/Helpers/appstyle.h"
#include "source/Model/Termin/paintedterm.h"

EdgeOld::EdgeOld(QObject* parent)
    : QObject(parent)
    , GraphEdge()
    , GraphicItem()
{}

EdgeOld::EdgeOld(PaintedTerm* toRoot, PaintedTerm* toLeaf, EdgeType type, QObject* parent)
    : QObject(parent)
    , GraphEdge(toRoot, toLeaf)
    , GraphicItem()
{
    mType = type;
}

QColor EdgeOld::getEdgeColor() const
{
    switch (mSelected) {
    case EdgeSelected::backward: return AppStyle::Colors::Edges::selected;
    case EdgeSelected::forward: return AppStyle::Colors::Edges::selectedAlt;
    default: break;
    }

    switch (mType) {
    case EdgeType::standart: return AppStyle::Colors::Edges::standard;
    case EdgeType::termin: return AppStyle::Colors::Edges::termin;
    case EdgeType::terminHardLink: return AppStyle::Colors::Edges::terminHardLink;
    case EdgeType::description: return AppStyle::Colors::Edges::description;
    case EdgeType::broken: return AppStyle::Colors::Edges::broken;
    case EdgeType::redundant: return AppStyle::Colors::Edges::standard;
    }

    return AppStyle::Colors::Edges::standard;
}

void EdgeOld::brokeEdge()
{
    cutOutFromSides();
    getRoot()->addBrokenEdge(this);

    mType = EdgeType::broken;
}

void EdgeOld::makeEdgeRedundant()
{
    cutOutFromSides();
    getLeaf()->addRedundantEdge(this);

    mType = EdgeType::redundant;
}

void EdgeOld::cutOutFromSides()
{
    getRoot()->removeEdgeToLeafs(this);
    getLeaf()->removeEdgeToRoots(this);
}

EdgeSelected EdgeOld::selectedType() const { return mSelected; }

bool EdgeOld::isSelected() const { return mSelected == EdgeSelected::forward || mSelected == EdgeSelected::backward; }

bool EdgeOld::isBroken() const { return mType == EdgeType::broken; }

bool EdgeOld::isRedundant() const { return mType == EdgeType::redundant; }

bool EdgeOld::isHard() const { return mType == EdgeType::terminHardLink; }

QRectF EdgeOld::edgeRect() const
{
    QPointF pt1 = rootPoint();
    QPointF pt2 = leafPoint();

    QRectF rc = QRectF(pt1, pt2);
    rc        = rc.normalized();
    return rc;
}

QPointF EdgeOld::rootPoint() const
{
    auto paintedTerm = static_cast<PaintedTerm*>(getRoot());
    return paintedTerm->getCenter(CoordType::scene);
}

QPointF EdgeOld::leafPoint() const
{
    auto paintedTerm = static_cast<PaintedTerm*>(getLeaf());
    return paintedTerm->getCenter(CoordType::scene);
}

EdgeOld::List EdgeOld::castToEdgeList(const GraphEdge::List& lst)
{
    EdgeOld::List edgeLst;
    for (auto graphEdg : lst) {
        edgeLst << dynamic_cast<EdgeOld*>(graphEdg);
    }
    return edgeLst;
}

void EdgeOld::setSelectedForward(bool value)
{
    auto resultSelection = value ? EdgeSelected::forward : EdgeSelected::none;
    if (mSelected != resultSelection) {
        mSelected = resultSelection;
        emit selectionChanged();
    }
}

void EdgeOld::setSelectedBackward(bool value)
{
    auto resultSelection = value ? EdgeSelected::backward : EdgeSelected::none;
    if (mSelected != resultSelection) {
        mSelected = resultSelection;
        emit selectionChanged();
    }
}
