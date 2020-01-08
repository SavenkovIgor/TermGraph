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

#pragma once

#include <QColor>
#include <QPointF>
#include <QLineF>

#include "source/Model/TerminEdge/graphedge.h"
#include "source/Model/Termin/paintedterm.h"
#include "source/Model/GraphicItem/graphicitem.h"
#include "source/Helpers/appstyle.h"

class PaintedTerm;

enum class EdgeType {
    standart,
    termin,
    description,
    broken
};

enum class EdgeSelected {
    none,
    forward,
    backward
};

class Edge : public GraphEdge, public GraphicItem
{
public:
    Edge(PaintedTerm* toRoot, PaintedTerm* toLeaf, EdgeType type = EdgeType::termin);
    ~Edge() override = default;

    QRectF edgeRect() const;

    int getLayerDistance();

    qreal getXProjection();
    qreal getYProjection();

    QPointF swapPointRoot = QPointF();
    QPointF swapPointLeaf = QPointF();
    QLineF getLine(bool swap = false);

    static EdgesList castToEdgeList(GraphEdge::List lst);  // TODO: Delete!!!

    // Paint width
    void setSelectedForward(bool value);
    void setSelectedBackward(bool value);

    QColor getEdgeColor() const;

    void brokeEdge();
    void cutOutFromSides();

    EdgeSelected selectedType() const;

private:
    EdgeSelected selected = EdgeSelected::none;
    EdgeType type = EdgeType::termin;
};
