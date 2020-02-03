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
#include <QLineF>
#include <QObject>
#include <QPointF>

#include "source/Model/GraphicItem/graphicitem.h"
#include "source/Model/TerminEdge/graphedge.h"

class PaintedTerm;

enum class EdgeType { standart, termin, description, broken };

enum class EdgeSelected { none, forward, backward };

class Edge : public QObject, public GraphEdge, public GraphicItem
{
    Q_OBJECT

    Q_PROPERTY(QPointF pt1 READ rootPoint)
    Q_PROPERTY(QPointF pt2 READ leafPoint)
    Q_PROPERTY(QColor color READ getEdgeColor)

public:
    using List = QList<Edge*>;

    explicit Edge(QObject* parent = nullptr);
    Edge(PaintedTerm* toRoot, PaintedTerm* toLeaf, EdgeType type = EdgeType::termin, QObject* parent = nullptr);
    ~Edge() override = default;

    QRectF edgeRect() const;

    int getLayerDistance();

    qreal getXProjection();
    qreal getYProjection();

    QPointF swapPointRoot = QPointF();
    QPointF swapPointLeaf = QPointF();
    QLineF  getLine(bool swap = false);

    QPointF rootPoint() const;
    QPointF leafPoint() const;
    QColor  getEdgeColor() const;

    static List castToEdgeList(GraphEdge::List lst);  // TODO: Delete!!!

    // Paint width
    void setSelectedForward(bool value);
    void setSelectedBackward(bool value);

    void brokeEdge();
    void cutOutFromSides();

    EdgeSelected selectedType() const;

private:
    EdgeSelected selected = EdgeSelected::none;
    EdgeType     type     = EdgeType::termin;
};
